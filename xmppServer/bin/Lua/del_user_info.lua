redis.call("SREM", "user:online", unpack(KEYS))
redis.call("DEL", unpack(KEYS))
