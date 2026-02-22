---@diagnostic disable: lowercase-global, undefined-global
local msg = require("mp.msg")
local utils = require("mp.utils")

-- #region settings
local settings = {
	key_mark_cut = "c",
	video_extension = "mp4",
	custom_output_path = "$HOME/Videos/Clips/",

	ffmpeg_custom_parameters = "",

	web = {
		key_mark_cut = "shift+c",
		audio_target_bitrate = 128, -- kbps
		video_target_file_size = 7.50, -- mb
		video_target_scale = "original"
	}
}
-- #endregion

-- #region globals
local vars = {
	path = nil,
	filename = nil,
	only_filename = nil,
	is_web_mark_pos = false,
	pos = {
		start_pos = nil,
		end_pos = nil,
		cut_duration = nil
	}
}
-- #endregion

-- #region utils
local function str_split(input, sep)
	sep = sep or "%s"
	local t = {}
	for str in string.gmatch(input, "([^"..sep.."]+)") do
		table.insert(t, str)
	end
	return t
end

local function to_timestamp(time)
	local hrs = math.floor(time / 3600)
	local mins = math.floor((time % 3600) / 60)
	local secs = math.floor(time % 60)
	local ms = (time % 1) * 1000
	return string.format("%02d:%02d:%02d.%03d", hrs, mins, secs, ms)
end

local function sanitize_filename(name)
	return name:gsub("[\\/:*?\"<>|]", "_")
end

local function reset_pos()
	vars.pos.start_pos = nil
	vars.pos.end_pos = nil
	vars.pos.cut_duration = nil
end

local function log(level, message, osd_delay)
	if osd_delay and osd_delay > 0 then
		mp.osd_message(message, osd_delay)
	end
	level(message)
end

local function log_file(message)
	local file = io.open("mpv-cut.log", "a")
	if file then
		file:write(message .. "\n")
		file:close()
	else
		msg.error("Cannot open mpv-cut.log")
	end
end

local function exec_native(args)
	log(msg.info, "Executing: "..table.concat(args, " "))
	local ret = mp.command_native({
		name = "subprocess",
		args = args,
		capture_stdout = true,
		capture_stderr = true,
		playback_only = false
	})
	if ret.status > 0 then
		local err = ret.stderr:gsub("^%s*(.-)%s*$", "%1")
		log(msg.error, "Error: "..err, 10)
		log_file(err)
	end
	return ret.status, ret.stdout, ret.stderr
end
-- #endregion

-- #region ffmpeg
local function ffmpeg_cut(start_time, end_time, input, output)
	local args
	if settings.ffmpeg_custom_parameters ~= "" and not vars.is_web_mark_pos then
		local custom_args = str_split(settings.ffmpeg_custom_parameters)
		args = {"ffmpeg", "-async", "1", "-y", "-i", input}
		for _, v in ipairs(custom_args) do table.insert(args, v) end
		for _, v in ipairs({"-ss", start_time, "-to", end_time, output}) do table.insert(args, v) end
	else
		args = {"ffmpeg", "-async", "1", "-y", "-ss", start_time, "-to", end_time,
		"-i", input, "-c:v", "copy", "-c:a", "aac", "-b:a", "320k", output}
	end
	local status = exec_native(args)
	return status == 0
end

local function ffmpeg_resize(input, output)
	local duration = math.abs(vars.pos.cut_duration or 0)
	local target_bitrate = (settings.web.video_target_file_size * 8192) / duration - settings.web.audio_target_bitrate
	if target_bitrate <= 0 then
		log(msg.error, "Calculated video bitrate <= 0!", 10)
		return false
	end
	local vb = string.format("%sk", math.floor(target_bitrate))
	local vf = "-vf"
	local scale = (settings.web.video_target_scale == "original")
	and "scale=iw:ih" or "scale="..settings.web.video_target_scale

	local first = {"ffmpeg", "-async", "1", "-y", "-i", input, "-c:v", "libx264", vf, scale,
	"-b:v", vb, "-pass", "1", "-an", "-f", "mp4", os.name=="windows" and "NUL" or "/dev/null"}
	local second = {"ffmpeg", "-async", "1", "-y", "-i", input, "-c:v", "libx264", vf, scale,
	"-b:v", vb, "-pass", "2", "-c:a", "aac", "-b:a", string.format("%sk", settings.web.audio_target_bitrate), output}

	if exec_native(first) ~= 0 or exec_native(second) ~= 0 then
		return false
	end

	-- cleanup pass files
	for _, f in ipairs({"ffmpeg2pass-0.log","ffmpeg2pass-0.log.mbtree"}) do
		local ok, err = os.remove(f)
		if not ok then log(msg.error, "Failed to delete "..f..": "..(err or "")) end
	end

	return true
end
-- #endregion

-- #region main
local function mark_pos(is_web)
	local now = mp.get_property_number("time-pos")
	if not vars.pos.start_pos then
		vars.pos.start_pos = now
		log(msg.info, "Marked start at "..to_timestamp(now), 3)
		return
	end

	vars.pos.end_pos = now
	vars.pos.cut_duration = vars.pos.end_pos - vars.pos.start_pos
	if vars.pos.cut_duration <= 0 then
		log(msg.error, "Invalid selection: end before start!", 3)
		reset_pos()
		return
	end

	local timestamp = os.date("%Y%m%d_%H%M%S")
	local base = sanitize_filename(vars.only_filename):gsub("%."..settings.video_extension.."$", "")
	local path = settings.custom_output_path
	local output_name = string.format("%s%s cut %s.%s", path~="" and path.."\\ " or "", base, timestamp, settings.video_extension)

	-- cut
	if not ffmpeg_cut(to_timestamp(vars.pos.start_pos), to_timestamp(vars.pos.end_pos), vars.path, output_name) then
		log(msg.error, "ffmpeg cut failed!", 10)
		reset_pos()
		return
	end

	-- optional resize
	if is_web then
		local resized_name = string.format("%s%s cutr %s.%s", path~="" and path.."\\ " or "", base, timestamp, settings.video_extension)
		log(msg.info, "Resizing...", 5)
		if not ffmpeg_resize(output_name, resized_name) then
			log(msg.error, "ffmpeg resize failed!", 10)
			reset_pos()
			return
		end
		os.remove(output_name)
		log(msg.info, "Saved: "..resized_name, 10)
	else
		log(msg.info, "Saved: "..output_name, 10)
	end

	reset_pos()
	vars.is_web_mark_pos = false
	mp.set_property("keep-open", "no")
end

local function web_mark_pos()
	vars.is_web_mark_pos = true
	mark_pos(true)
end
-- #endregion

-- #region events
mp.register_event("file-loaded", function()
	vars.path = mp.get_property("path")
	vars.filename = mp.get_property("filename")
	vars.only_filename = mp.get_property("filename")
	-- mp.set_property("keep-open", "always")
end)

mp.add_key_binding(settings.key_mark_cut, "mark_pos", function() mark_pos(false) end)
mp.add_key_binding(settings.web.key_mark_cut, "web_mark_pos", web_mark_pos)
-- #endregion
