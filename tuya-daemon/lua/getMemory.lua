local ubusAvailable = pcall(require, "ubus")

conn = nil

function free_memory()
    local response = conn:call("system", "info", {})
    local memory = response.memory
    
    return "{\"free_memory\":" .. response.memory.free .. "}"
end

function total_memory()
    local response = conn:call("system", "info", {})
    local memory = response.memory
    
    return "{\"total_memory\":" .. response.memory.total .. "}"
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
    return free_memory(), total_memory()
end

function deinit()
    if conn then
        conn:close()
    end
end