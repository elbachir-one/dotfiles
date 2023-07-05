// somebar - dwl bar
// See LICENSE file for copyright and license details.

#pragma once
#include <optional>
#include <string>
#include <vector>
#include <wayland-client.h>
#include "wlr-layer-shell-unstable-v1-client-protocol.h"
#include "common.hpp"
#include "shm_buffer.hpp"

class BarComponent {
	std::unique_ptr<std::string> _text;
public:
	BarComponent();
	explicit BarComponent(wl_unique_ptr<PangoLayout> layout);
	int width() const;
	void setText(const std::string& text);
	wl_unique_ptr<PangoLayout> pangoLayout;
	int x {0};
};

struct Tag {
	int state;
	int numClients;
	int focusedClient;
	BarComponent component;
};

struct Monitor;
class Bar {
	static const zwlr_layer_surface_v1_listener _layerSurfaceListener;
	static const wl_callback_listener _frameListener;

	wl_unique_ptr<wl_surface> _surface;
	wl_unique_ptr<zwlr_layer_surface_v1> _layerSurface;
	wl_unique_ptr<PangoContext> _pangoContext;
	std::optional<ShmBuffer> _bufs;
	std::vector<Tag> _tags;
	BarComponent _layoutCmp, _titleCmp, _statusCmp;
	bool _selected;
	bool _invalid {false};

	// only vaild during render()
	cairo_t* _painter {nullptr};
	int _x;
	ColorScheme _colorScheme;

	void layerSurfaceConfigure(uint32_t serial, uint32_t width, uint32_t height);
	void render();
	void renderTags();
	void renderStatus();

	// low-level rendering
	void setColorScheme(const ColorScheme& scheme, bool invert = false);
	void beginFg();
	void beginBg();
	void renderComponent(BarComponent& component);
	BarComponent createComponent(const std::string& initial = {});
public:
	Bar();
	const wl_surface* surface() const;
	bool visible() const;
	void show(wl_output* output);
	void hide();
	void setTag(int tag, int state, int numClients, int focusedClient);
	void setSelected(bool selected);
	void setLayout(const std::string& layout);
	void setTitle(const std::string& title);
	void setStatus(const std::string& status);
	void invalidate();
	void click(Monitor* mon, int x, int y, int btn);
};
