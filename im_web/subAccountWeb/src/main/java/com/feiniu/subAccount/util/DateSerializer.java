package com.feiniu.subAccount.util;

import java.io.IOException;
import java.util.Date;

import org.codehaus.jackson.JsonGenerator;
import org.codehaus.jackson.JsonProcessingException;
import org.codehaus.jackson.map.JsonSerializer;
import org.codehaus.jackson.map.SerializerProvider;


public class DateSerializer extends JsonSerializer<Date> {

    public void serialize(Date value, JsonGenerator jgen,
                          SerializerProvider provider)
            throws IOException, JsonProcessingException {
        jgen.writeString(FormatUtils.formatDate(value));
    }
}
