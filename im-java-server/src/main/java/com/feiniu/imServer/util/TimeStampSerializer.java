package com.feiniu.imServer.util;

import org.codehaus.jackson.JsonGenerator;
import org.codehaus.jackson.JsonProcessingException;
import org.codehaus.jackson.map.JsonSerializer;
import org.codehaus.jackson.map.SerializerProvider;

import java.io.IOException;
import java.sql.Timestamp;


public class TimeStampSerializer extends JsonSerializer<Timestamp> {
	  
    public void serialize(Timestamp value, JsonGenerator jgen, SerializerProvider provider) throws IOException, JsonProcessingException {
                jgen.writeString(FormatUtils.formatTime(value));
        }  
}
