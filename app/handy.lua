-- handy text editor
-- Copyright (c) 2021 David Capello

local log

function init()
  log = io.open("handy.log", "w")
  log:write('init\n')
end

function exit()
  log:write('exit\n')
  log:close(f)
end
