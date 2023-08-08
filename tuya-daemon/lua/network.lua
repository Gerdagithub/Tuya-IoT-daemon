local ubusAvailable = pcall(require, "ubus")

conn = nil

function get_ipv4_address()
    local response = conn:call("network.interface.lan", "status", {})

    return "{\"ip_address\":" .. "\"" .. response["ipv4-address"][1].address .. "\"}" 
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
    
    return get_ipv4_address()
end

function deinit()
    if conn then
        conn:close()
    end
end