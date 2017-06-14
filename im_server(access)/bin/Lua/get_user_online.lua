local count = redis.call("SCARD", "user:online")
local user_set = redis.call("SRANDMEMBER", "user:online", KEYS[1])
local ret = {}
if #user_set ~= 0 then
	for k, v in pairs(user_set) do
		local user_tb = redis.call("HMGET", v, "sexy", "status", "rule", "username", "nickname")
		if #user_tb ~= 0 then
			table.insert(user_tb, 1, v)
			table.insert(ret, user_tb)
		end
	end
end
table.insert(ret, count)
return ret
