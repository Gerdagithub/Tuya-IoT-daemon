map = Map("tuya_daemon", "Tuya IoT")

section = map:section(NamedSection, "tuya_daemon_sct", "tuya_daemon", "Tuya IoT")

flag = section:option(Flag, "enable", "Enable", "Enable program")

productId = section:option(Value, "product_id", "Product ID")
deviceId = section:option(Value, "device_id", "Device ID")
deviceId.maxlength = 30
deviceSecret = section:option(Value, "secret", "Device Secret")

return map