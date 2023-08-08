local ubusAvailable = pcall(require, "ubus")

conn = nil

function system_release_info() 
    local response = conn:call("system", "board", {})

    return "{\"description\":" .. "\"" .. response.release.description .. "\"}" 
end

function init()
    if not ubusAvailable then 
        return
    end
    
    conn = ubus.connect()
    if not conn then
        error("Failed to connect to ubusd")
    end
end

function get_data()
    return system_release_info()
end

function deinit()
    if conn then
        conn:close()
    end
end