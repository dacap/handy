-- handy text editor
-- Copyright (c) 2021-2024 David Capello

local logfile
function log(s)
  if not logfile then
    logfile = io.open("handy.log", "w")
    logfile:write(s)
  end
end

function is_file(fn)
  local f = io.open(fn, "r")
  if f then
    io.close(f)
    return true
  end
  return false
end

function init()
  local user_file = 'handy-user.lua'
  if is_file(user_file) then
    dofile(user_file)
  end
end

function exit()
  if logfile then
    logfile:close()
  end
end
