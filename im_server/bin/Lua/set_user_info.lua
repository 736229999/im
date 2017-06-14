local field_cnt = 12
for i = 1, #KEYS do
	local fid = field_cnt * (i - 1)
	redis.call("HMSET", KEYS[i], "flag", ARGV[fid + 1], "status", ARGV[fid + 2],
		"rule", ARGV[fid + 3], "username", ARGV[fid + 4], "nickname", ARGV[fid + 5],
		"clientip", ARGV[fid + 6], "clientport", ARGV[fid + 7], "sexy", ARGV[fid + 8],
		"serverip", ARGV[fid + 9], "lanip", ARGV[fid + 10], "sessionkey", ARGV[fid + 11],
		"p2psessionkey", ARGV[fid + 12])
	local status = tonumber(ARGV[fid + 2])
	if status ~= 2 and status ~= 3 then
		redis.call("SADD", "user:online", KEYS[i])
	end
end
return {ok = "OK"}
