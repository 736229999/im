package com.feiniu.fnSubAccount.service.external;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.feiniu.common.core.HttpRemoteClient;
import com.feiniu.common.core.SystemEnv;
import com.google.gson.Gson;
import org.springframework.stereotype.Service;

/**
 * 消息中心接口调用
 */
@SuppressWarnings({ "unused","unchecked", "rawtypes" })
@Service
public class InfoCenterApiClient {

	private static final Logger logger = LoggerFactory.getLogger(InfoCenterApiClient.class);
	
	private static final Gson gson = new Gson();

	public static class Message {

		/**
		 * 消息名称
		 */
		private String infoTitle;
		/**
		 * 消息模板: ##订单编号##
		 */
		private String infoTemplate;
		/**
		 * 消息项
		 */
		private List<MessageItem> items;

		/**
		 * 模块: BA
		 */
		private String module;
		/**
		 * 发送人ID
		 */
		private String authorId;
		/**
		 * 发送人名称
		 */
		private String authorName;

		public String getInfoTitle() {
			return infoTitle;
		}

		public void setInfoTitle(String infoTitle) {
			this.infoTitle = infoTitle;
		}

		public String getInfoTemplate() {
			return infoTemplate;
		}

		public void setInfoTemplate(String infoTemplate) {
			this.infoTemplate = infoTemplate;
		}

		public List<MessageItem> getItems() {
			return items;
		}

		public void setItems(List<MessageItem> items) {
			this.items = items;
		}

		public String getModule() {
			return module;
		}

		public void setModule(String module) {
			this.module = module;
		}

		public String getAuthorId() {
			return authorId;
		}

		public void setAuthorId(String authorId) {
			this.authorId = authorId;
		}

		public String getAuthorName() {
			return authorName;
		}

		public void setAuthorName(String authorName) {
			this.authorName = authorName;
		}
	}

	public static class MessageItem {
		private String receiver;
		private String[] infoValues;

		public MessageItem(String receiver, String[] infoValues) {
			this.receiver = receiver;
			this.infoValues = infoValues;
		}

		public String getReceiver() {
			return receiver;
		}

		public String[] getInfoValues() {
			return infoValues;
		}
	}

	public static enum SendType {
		SMS("1"), EMAIL("2"), INNERMAIL("3"), MOMO("4");
		private String code;

		SendType(String code) {
			this.code = code;
		}

		public String getCode() {
			return code;
		}
	}

	private static Map<String, Object> createParamMap(Message message, SendType sendType) {
		Map<String, Object> map = new HashMap<String, Object>();
		map.put("content", message.getInfoTemplate());
		String recive_str = "";
		List<String> valueList = new ArrayList<String>();
		List<MessageItem> items = message.getItems();
		for (MessageItem item : items) {
			recive_str += item.getReceiver() + "\n";
			String valueStr = item.getReceiver();
			String[] values = item.getInfoValues();
			if (values != null) {
				for (String val : values) {
					valueStr += "," + val;
				}
			}
			valueList.add(valueStr);
		}
		map.put("recive_str", recive_str);
		map.put("sendType", sendType.getCode());
		map.put("contentValue", gson.toJson(valueList));
		map.put("infoTitle", message.getInfoTitle());
		map.put("title", "");
		map.put("author_id", message.getAuthorId());
		map.put("author_name", message.getAuthorName());
		// COMPLETE(0) 完整内容, TEMPLATE(1) 模板,TEMPTEMPLATE(2) 临时模板
		map.put("contentType", 2);
		// 优先级 1~9
		map.put("priority", 1);
		//
		map.put("info_type", "4");
		map.put("sendId", message.getModule() + System.currentTimeMillis());
		return map;
	}

	/**
	 * 发邮件 MessageItem receiver: 邮箱
	 */
	public static void sendEmail(Message message) {
		sendMessage(message, SendType.EMAIL);
	}

	/**
	 * 发短信 MessageItem receiver: 手机号
	 */
	public static void sendSMS(Message message) {
		sendMessage(message, SendType.SMS);
	}

	/**
	 * 发站内信 MessageItem receiver: 商家Id
	 */
	public static void sendInnerMail(Message message) {
		sendMessage(message, SendType.INNERMAIL);
	}

	private static void sendMessage(Message message, SendType sendType) {
		if (message.getModule() == null) {
			message.setModule("");
		}
		if (message.getInfoTemplate() == null || message.getItems() == null) {
			return;
		}
		String url = SystemEnv.getProperty("feiniu.infoCenterBackendHost", "http://infocenterbackend.beta.fn.com:8080") + "/messageSend/saveInfoObjByOther.do";
		try {
			String response = HttpRemoteClient.doPost(url, createParamMap(message, sendType));
			logger.info(url + " is call :" + response);
		} catch (Exception e) {
			logger.error(url + " is call error!", e);
		}
	}



}
