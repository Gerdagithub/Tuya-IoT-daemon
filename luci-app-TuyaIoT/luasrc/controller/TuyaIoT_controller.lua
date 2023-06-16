module("luci.controller.TuyaIoT_controller", package.seeall)

function index()
	entry({"admin", "services", "TuyaIoT_model"}, cbi("TuyaIoT_model"), _("Tuya IoT"),105)
end
