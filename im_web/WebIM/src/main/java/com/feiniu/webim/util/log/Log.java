package com.feiniu.webim.util.log;


import com.feiniu.webim.config.ConfigManager;
import org.apache.log4j.Logger;
import org.apache.log4j.PropertyConfigurator;


/**
 * 日志记录类
 * 封装Log4j,可以在此类中更换日志记录
 * @author zb
 *
 */
public class Log {
 
	/**
	 * 初始化,日志
	 */
	public static void init(){
		PropertyConfigurator.configure(ConfigManager.getProperty("webPath")+ ConfigManager.getProperty("logfile"));
	}

	public static void debug(Logger logger, String message) {
		debug(logger, message, null);
	}

	public static void debug(Logger logger, String message, Throwable throwable) {
		if (logger != null && logger.isDebugEnabled()) {
			if (throwable != null) {
				logger.debug(message, throwable);
			} else {
				logger.debug(message);
			}
		}
	}

}
