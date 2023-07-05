// somebar - dwl barbar
// See LICENSE file for copyright and license details.

#include <wayland-client-protocol.h>
#include <pango/pangocairo.h>
#include "bar.hpp"
#include "cairo.h"
#include "config.hpp"
#include "pango/pango-font.h"
#include "pango/pango-fontmap.h"
#include "pango/pango-layout.h"

const zwlr_layer_surface_v1_listener Bar::_layerSurfaceListener = {
	[](void* owner, zwlr_layer_surface_v1*, uint32_t serial, uint32_t width, uint32_t height)
	{
		static_cast<Bar*>(owner)->layerSurfaceConfigure(serial, width, height);
	}
};
const wl_callback_listener Bar::_frameListener = {
	[](void* owner, wl_callback* cb, uint32_t)
	{
		static_cast<Bar*>(owner)->render();
		wl_callback_destroy(cb);
	}
};

struct Font {
	PangoFontDescription* description;
	int height {0};
};
static Font getFont()
{
	auto fontMap = pango_cairo_font_map_get_default();
	if (!fontMap) {
		die("pango_cairo_font_map_get_default");
	}
	auto fontDesc = pango_font_description_from_string(font);
	if (!fontDesc) {
		die("pango_font_description_from_string");
	}
	auto tempContext = pango_font_map_create_context(fontMap);
	if (!tempContext) {
		die("pango_font_map_create_context");
	}
	auto font = pango_font_map_load_font(fontMap, tempContext, fontDesc);
	if (!font) {
		die("pango_font_map_load_font");
	}
	auto metrics = pango_font_get_metrics(font, pango_language_get_default());
	if (!metrics) {
		die("pango_font_get_metrics");
	}

	auto res = Font {};
	res.description = fontDesc;
	res.height = PANGO_PIXELS(pango_font_metrics_get_height(metrics));

	pango_font_metrics_unref(metrics);
	g_object_unref(font);
	g_object_unref(tempContext);
	return res;
}
static Font barfont = getFont();

BarComponent::BarComponent() { }
BarComponent::BarComponent(wl_unique_ptr<PangoLayout> layout)
	: pangoLayout {std::move(layout)}
{
}

int BarComponent::width() const
{
	int w, h;
	pango_layout_get_size(pangoLayout.get(), &w, &h);
	return PANGO_PIXELS(w);
}

void BarComponent::setText(const std::string& text)
{
	_text = std::make_unique<std::string>(text);
	pango_layout_set_text(pangoLayout.get(), _text->c_str(), _text->size());
}

Bar::Bar()
{
	_pangoContext.reset(pango_font_map_create_context(pango_cairo_font_map_get_default()));
	if (!_pangoContext) {
		die("pango_font_map_create_context");
	}
	for (const auto& tagName : tagNames) {
		_tags.push_back({ TagState::None, 0, 0, createComponent(tagName) });
	}
	_layoutCmp = createComponent();
	_titleCmp = createComponent();
	_statusCmp = createComponent();
}

const wl_surface* Bar::surface() const
{
	return _surface.get();
}

bool Bar::visible() const
{
	return _surface.get();
}

void Bar::show(wl_output* output)
{
	if (visible()) {
		return;
	}
	_surface.reset(wl_compositor_create_surface(compositor));
	_layerSurface.reset(zwlr_layer_shell_v1_get_layer_surface(wlrLayerShell,
		_surface.get(), output, ZWLR_LAYER_SHELL_V1_LAYER_BOTTOM, "net.tapesoftware.Somebar"));
	zwlr_layer_surface_v1_add_listener(_layerSurface.get(), &_layerSurfaceListener, this);
	auto anchor = topbar ? ZWLR_LAYER_SURFACE_V1_ANCHOR_TOP : ZWLR_LAYER_SURFACE_V1_ANCHOR_BOTTOM;
	zwlr_layer_surface_v1_set_anchor(_layerSurface.get(),
		anchor | ZWLR_LAYER_SURFACE_V1_ANCHOR_LEFT | ZWLR_LAYER_SURFACE_V1_ANCHOR_RIGHT);

	auto barSize = barfont.height + paddingY * 2;
	zwlr_layer_surface_v1_set_size(_layerSurface.get(), 0, barSize);
	zwlr_layer_surface_v1_set_exclusive_zone(_layerSurface.get(), barSize);
	wl_surface_commit(_surface.get());
}

void Bar::hide()
{
	if (!visible()) {
		return;
	}
	_layerSurface.reset();
	_surface.reset();
	_bufs.reset();
}

void Bar::setTag(int tag, int state, int numClients, int focusedClient)
{
	auto& t = _tags[tag];
	t.state = state;
	t.numClients = numClients;
	t.focusedClient = focusedClient;
}

void Bar::setSelected(bool selected)
{
	_selected = selected;
}
void Bar::setLayout(const std::string& layout)
{
	_layoutCmp.setText(layout);
}
void Bar::setTitle(const std::string& title)
{
	_titleCmp.setText(title);
}
void Bar::setStatus(const std::string& status)
{
	_statusCmp.setText(status);
}

void Bar::invalidate()
{
	if (_invalid || !visible()) {
		return;
	}
	_invalid = true;
	auto frame = wl_surface_frame(_surface.get());
	wl_callback_add_listener(frame, &_frameListener, this);
	wl_surface_commit(_surface.get());
}

void Bar::click(Monitor* mon, int x, int, int btn)
{
	Arg arg = {0};
	Arg* argp = nullptr;
	int control = ClkNone;
	if (x > _statusCmp.x) {
		control = ClkStatusText;
	} else if (x > _titleCmp.x) {
		control = ClkWinTitle;
	} else if (x > _layoutCmp.x) {
		control = ClkLayoutSymbol;
	} else for (auto tag = _tags.size()-1; tag >= 0; tag--) {
		if (x > _tags[tag].component.x) {
			control = ClkTagBar;
			arg.ui = 1<<tag;
			argp = &arg;
			break;
		}
	}
	for (const auto& button : buttons) {
		if (button.control == control && button.btn == btn) {
			button.func(*mon, *(argp ? argp : &button.arg));
			return;
		}
	}
}

void Bar::layerSurfaceConfigure(uint32_t serial, uint32_t width, uint32_t height)
{
	zwlr_layer_surface_v1_ack_configure(_layerSurface.get(), serial);
	if (_bufs && width == _bufs->width && height == _bufs->height) {
		return;
	}
	_bufs.emplace(width, height, WL_SHM_FORMAT_XRGB8888);
	render();
}

void Bar::render()
{
	if (!_bufs) {
		return;
	}
	auto img = wl_unique_ptr<cairo_surface_t> {cairo_image_surface_create_for_data(
		_bufs->data(),
		CAIRO_FORMAT_ARGB32,
		_bufs->width,
		_bufs->height,
		_bufs->stride
		)};
	auto painter = wl_unique_ptr<cairo_t> {cairo_create(img.get())};
	_painter = painter.get();
	pango_cairo_update_context(_painter, _pangoContext.get());
	_x = 0;

	renderTags();
	setColorScheme(_selected ? colorActive : colorInactive);
	renderComponent(_layoutCmp);
	renderComponent(_titleCmp);
	renderStatus();

	_painter = nullptr;
	wl_surface_attach(_surface.get(), _bufs->buffer(), 0, 0);
	wl_surface_damage(_surface.get(), 0, 0, _bufs->width, _bufs->height);
	wl_surface_commit(_surface.get());
	_bufs->flip();
	_invalid = false;
}

void Bar::renderTags()
{
	for (auto &tag : _tags) {
		setColorScheme(
			tag.state & TagState::Active ? colorActive : colorInactive,
			tag.state & TagState::Urgent);
		renderComponent(tag.component);
		auto indicators = std::min(tag.numClients, static_cast<int>(_bufs->height/2));
		for (auto ind = 0; ind < indicators; ind++) {
			auto w = ind == tag.focusedClient ? 7 : 1;
			cairo_move_to(_painter, tag.component.x, ind*2+0.5);
			cairo_rel_line_to(_painter, w, 0);
			cairo_close_path(_painter);
			cairo_set_line_width(_painter, 1);
			cairo_stroke(_painter);
		}
	}
}

void Bar::renderStatus()
{
	pango_cairo_update_layout(_painter, _statusCmp.pangoLayout.get());
	beginBg();
	auto start = _bufs->width - _statusCmp.width() - paddingX*2;
	cairo_rectangle(_painter, _x, 0, _bufs->width-_x+start, _bufs->height);
	cairo_fill(_painter);

	_x = start;
	renderComponent(_statusCmp);
}

void Bar::setColorScheme(const ColorScheme& scheme, bool invert)
{
	_colorScheme = invert
		? ColorScheme {scheme.bg, scheme.fg}
		: ColorScheme {scheme.fg, scheme.bg};
}
static void setColor(cairo_t* painter, const Color& color)
{
	cairo_set_source_rgba(painter,
		color.r/255.0, color.g/255.0, color.b/255.0, color.a/255.0);
}
void Bar::beginFg()
{
	setColor(_painter, _colorScheme.fg);
}
void Bar::beginBg()
{
	setColor(_painter, _colorScheme.bg);
}

void Bar::renderComponent(BarComponent& component)
{
	pango_cairo_update_layout(_painter, component.pangoLayout.get());
	auto size = component.width() + paddingX*2;
	component.x = _x;

	beginBg();
	cairo_rectangle(_painter, _x, 0, size, _bufs->height);
	cairo_fill(_painter);
	cairo_move_to(_painter, _x+paddingX, paddingY);

	beginFg();
	pango_cairo_show_layout(_painter, component.pangoLayout.get());
	_x += size;
}

BarComponent Bar::createComponent(const std::string &initial)
{
	auto layout = pango_layout_new(_pangoContext.get());
	pango_layout_set_font_description(layout, barfont.description);
	auto res = BarComponent {wl_unique_ptr<PangoLayout> {layout}};
	res.setText(initial);
	return res;
}
