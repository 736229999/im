redis.call('ZADD', KEYS[1], KEYS[2], KEYS[3])
local len = tonumber(redis.call('ZCARD', KEYS[1]))
if len > 1000 then
	redis.call('ZREMRANGEBYRANK', KEYS[1], '0', tostring(len - 1001))
end
return {ok='OK'}
