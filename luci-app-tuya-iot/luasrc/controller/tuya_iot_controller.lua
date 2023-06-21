module("luci.controller.tuya_iot_controller", package.seeall)

function index()
	entry({"admin", "services", "tuya_iot_model"}, cbi("tuya_iot_model"), _("Tuya IoT"),105)
end
