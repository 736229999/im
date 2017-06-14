package com.feiniu.webim.config;

import org.apache.log4j.Logger;
import org.dom4j.*;
import org.dom4j.io.SAXReader;
import org.xml.sax.ErrorHandler;
import org.xml.sax.SAXException;
import org.xml.sax.SAXParseException;

import java.io.InputStream;
import java.util.HashMap;

public class XMLConfig {

	public static final Logger LOGGER = Logger.getLogger(XMLConfig.class);
	private HashMap<String, String> properties = new HashMap<String, String>();
	
	
	public XMLConfig(){}
	
	public XMLConfig(InputStream in) throws DocumentException{
		this.load(in);
	}
	
	/**
	 * 加载配置文件并解析
	 * @param path
	 * @throws org.dom4j.DocumentException
	 */
	public void load(InputStream in) throws DocumentException{
		SAXReader reader = new SAXReader();
		
		WebVisitor web = new WebVisitor();
		Document doc;
		doc = reader.read(in);
		doc.accept(web);
		reader.setErrorHandler(new ErrorHandler() {
			
			@Override
			public void warning(SAXParseException exception) throws SAXException {
				LOGGER.warn("配置文件解析出错:行"+exception.getLineNumber()+","+exception.getMessage());
			}
			
			@Override
			public void fatalError(SAXParseException exception) throws SAXException {
				LOGGER.fatal("配置文件解析出错:行"+exception.getLineNumber()+","+exception.getMessage());
			}
			
			@Override
			public void error(SAXParseException exception) throws SAXException {
				LOGGER.error("配置文件解析出错:行"+exception.getLineNumber()+","+exception.getMessage());
			}
		});
	}

	/**
	 * 设置属性配置
	 * @param key 属性名
	 * @param value 属性值
	 */
	public void setProperty(String key,String value){
		properties.put(key, value);
	}
	
	/**
	 * 获取属性配置
	 * @param key 属性名
	 * @return 属性值
	 */ 
	public String getProperty(String key){
		return properties.get(key);
	}
	
	public boolean containsKey(String key){
		return properties.containsKey(key);
	}
	
	/**
	 * 使用visitor 方式简单解析 xml配置文件
	 * @author zb
	 *
	 */
	private class WebVisitor extends VisitorSupport{
		
		//保存正在处理的节点
		private String currentELement;
		
		@Override
		public void visit(Element node){
				
			if(node.getName().equals("property")){
				currentELement = node.getText();
			}else{
				currentELement = null;
			}
				
		}
		@Override
		public void visit(Attribute attr){
			
			if(currentELement != null && attr.getName().equals("name")){
				properties.put(attr.getValue(), currentELement);
			}
			
		}
		
	}
	
	
}
