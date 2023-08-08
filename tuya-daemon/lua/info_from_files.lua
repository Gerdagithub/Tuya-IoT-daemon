function read_numbers_from_file(filename)
    local file = assert(io.open(filename, "r"))

    local json = "{\"numbers\":["
    local notFirst = false

    for line in file:lines() do
        if notFirst then
           json = json .. ","
        else
            notFirst = true            
        end
        json = json .. line
    end

    json = json .. "]}"
    file:close()
     
    return json
end

function get_data()
    return read_numbers_from_file("/usr/bin/tuya-daemon/files_for_lua/numbers_arr.txt")
    -- return read_numbers_from_file("/home/studentas/Documents/rutx_src/package/tuya-daemon2/files_for_lua/numbers_arr.txt")
end