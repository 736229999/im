for i = 1, #KEYS do
	redis.call("HSET", KEYS[i], "status", ARGV[i])
	local status = tonumber(ARGV[i])
	if status == 2 or status == 3 then
		redis.call("SREM", "user:online", KEYS[i])
	else
		redis.call("SADD", "user:online", KEYS[i])
	end
end
return {ok = "OK"}
