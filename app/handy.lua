-- handy text editor
-- Copyright (c) 2021-2024 David Capello

local logfile
function log(s)
  if not logfile then
    logfile = io.open("handy.log", "w")
    logfile:write(s)
  end
end

function init()
  -- Do nothing
end

function exit()
  if logfile then
    logfile:close()
  end
end
