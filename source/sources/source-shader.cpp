// AUTOGENERATED COPYRIGHT HEADER START
// Copyright (C) 2019-2023 Michael Fabian 'Xaymar' Dirks <info@xaymar.com>
// Copyright (C) 2021 coolsoft.rf <coolsoft.rf@gmail.com>
// AUTOGENERATED COPYRIGHT HEADER END

#include "source-shader.hpp"
#include "strings.hpp"
#include "obs/gs/gs-helper.hpp"
#include "util/util-logging.hpp"

#include "warning-disable.hpp"
#include <stdexcept>
#include "warning-enable.hpp"

#ifdef _DEBUG
#define ST_PREFIX "<%s> "
#define D_LOG_ERROR(x, ...) P_LOG_ERROR(ST_PREFIX##x, __FUNCTION_SIG__, __VA_ARGS__)
#define D_LOG_WARNING(x, ...) P_LOG_WARN(ST_PREFIX##x, __FUNCTION_SIG__, __VA_ARGS__)
#define D_LOG_INFO(x, ...) P_LOG_INFO(ST_PREFIX##x, __FUNCTION_SIG__, __VA_ARGS__)
#define D_LOG_DEBUG(x, ...) P_LOG_DEBUG(ST_PREFIX##x, __FUNCTION_SIG__, __VA_ARGS__)
#else
#define ST_PREFIX "<source::shader> "
#define D_LOG_ERROR(...) P_LOG_ERROR(ST_PREFIX __VA_ARGS__)
#define D_LOG_WARNING(...) P_LOG_WARN(ST_PREFIX __VA_ARGS__)
#define D_LOG_INFO(...) P_LOG_INFO(ST_PREFIX __VA_ARGS__)
#define D_LOG_DEBUG(...) P_LOG_DEBUG(ST_PREFIX __VA_ARGS__)
#endif

#define ST_I18N "Source.Shader"

using namespace streamfx::source::shader;

static constexpr std::string_view HELP_URL = "https://github.com/Xaymar/obs-StreamFX/wiki/Source-Filter-Transition-Shader";

shader_instance::shader_instance(obs_data_t* data, obs_source_t* self) : obs::source_instance(data, self), _fx()
{
	_fx = std::make_shared<::streamfx::gfx::shader::shader>(self, ::streamfx::gfx::shader::shader_mode::Source);

	update(data);
}

shader_instance::~shader_instance() {}

uint32_t shader_instance::get_width()
{
	return _fx->width();
}

uint32_t shader_instance::get_height()
{
	return _fx->height();
}

void shader_instance::properties(obs_properties_t* props)
{
	_fx->properties(props);
}

void shader_instance::load(obs_data_t* data)
{
	_fx->update(data);
}

void shader_instance::update(obs_data_t* data)
{
	_fx->update(data);
}

void shader_instance::video_tick(float_t sec_since_last)
{
	if (_fx->tick(sec_since_last)) {
		obs_data_t* data = obs_source_get_settings(_self);
		_fx->update(data);
		obs_data_release(data);
	}

	obs_video_info ovi;
	obs_get_video_info(&ovi);
	_fx->set_size(ovi.base_width, ovi.base_height);
}

void shader_instance::video_render(gs_effect_t* effect)
{
	if (!_fx) {
		return;
	}

#if defined(ENABLE_PROFILING) && !defined(D_PLATFORM_MAC) && _DEBUG
	streamfx::obs::gs::debug_marker gdmp{streamfx::obs::gs::debug_color_source, "Shader Source '%s'", obs_source_get_name(_self)};
#endif

	_fx->prepare_render();
	_fx->render(effect);
}

void streamfx::source::shader::shader_instance::show()
{
	_fx->set_visible(true);
}

void streamfx::source::shader::shader_instance::hide()
{
	_fx->set_visible(false);
}

void streamfx::source::shader::shader_instance::activate()
{
	_fx->set_active(true);
}

void streamfx::source::shader::shader_instance::deactivate()
{
	_fx->set_active(false);
}

shader_factory::shader_factory()
{
	_info.id           = S_PREFIX "source-shader";
	_info.type         = OBS_SOURCE_TYPE_INPUT;
	_info.output_flags = OBS_SOURCE_VIDEO | OBS_SOURCE_CUSTOM_DRAW;

	support_activity_tracking(true);
	support_visibility_tracking(true);
	finish_setup();
	register_proxy("obs-stream-effects-source-shader");
}

shader_factory::~shader_factory() {}

const char* shader_factory::get_name()
{
	return D_TRANSLATE(ST_I18N);
}

void shader_factory::get_defaults2(obs_data_t* data)
{
	::streamfx::gfx::shader::shader::defaults(data);
}

obs_properties_t* shader_factory::get_properties2(shader_instance* data)
{
	auto pr = obs_properties_create();
	obs_properties_set_param(pr, data, nullptr);

#ifdef ENABLE_FRONTEND
	{
		obs_properties_add_button2(pr, S_MANUAL_OPEN, D_TRANSLATE(S_MANUAL_OPEN), streamfx::source::shader::shader_factory::on_manual_open, nullptr);
	}
#endif

	if (data) {
		reinterpret_cast<shader_instance*>(data)->properties(pr);
	}

	return pr;
}

#ifdef ENABLE_FRONTEND
bool shader_factory::on_manual_open(obs_properties_t* props, obs_property_t* property, void* data)
{
	try {
		streamfx::open_url(HELP_URL);
		return false;
	} catch (const std::exception& ex) {
		D_LOG_ERROR("Failed to open manual due to error: %s", ex.what());
		return false;
	} catch (...) {
		D_LOG_ERROR("Failed to open manual due to unknown error.", "");
		return false;
	}
}
#endif

std::shared_ptr<shader_factory> _source_shader_factory_instance = nullptr;

void streamfx::source::shader::shader_factory::initialize()
{
	try {
		if (!_source_shader_factory_instance)
			_source_shader_factory_instance = std::make_shared<shader_factory>();
	} catch (const std::exception& ex) {
		D_LOG_ERROR("Failed to initialize due to error: %s", ex.what());
	} catch (...) {
		D_LOG_ERROR("Failed to initialize due to unknown error.", "");
	}
}

void streamfx::source::shader::shader_factory::finalize()
{
	_source_shader_factory_instance.reset();
}

std::shared_ptr<shader_factory> streamfx::source::shader::shader_factory::get()
{
	return _source_shader_factory_instance;
}
