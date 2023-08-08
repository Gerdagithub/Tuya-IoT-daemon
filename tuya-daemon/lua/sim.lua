local ubusAvailable = pcall(require, "ubus")

conn = nil

function get_sim_number()
    local response = conn:call("sim", "get", {})

    return "{\"sim\":" .. response.sim .. "}"
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
    return get_sim_number()
end

function deinit()
    if conn then
        conn:close()
    end
end