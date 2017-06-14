package com.feiniu.fnSubAccount.service;

import com.feiniu.fnSubAccount.util.json.JsonUtil;
import org.apache.commons.lang.StringUtils;
import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;
import org.jsoup.nodes.Element;
import org.jsoup.select.Elements;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Author: morningking
 * Date: 2015/10/20
 * Contact: 243717042@qq.com
 */
@Transactional
@Service
public class HomePageService {
    @Autowired
    private KefuIndexConfigService kefuIndexConfigService;
    @Autowired
    private AfterSaleBoardService afterSaleBoardConfigService;

    private static final Logger LOGGER = LoggerFactory.getLogger(HomePageService.class);

    public void publish() {
        kefuIndexConfigService.syncEditToRuntime();
        afterSaleBoardConfigService.syncEditToRuntime();
    }

    public boolean doFetchFooterOfFeiniuIndex(String selector) {
        Document document = null;
        try {
            document = Jsoup.connect("http://www.feiniu.com").get();
        } catch (Exception e) {
            LOGGER.error("抓取首页内容失败", e);
            return false;
        }

        Map<String, Object> indexJsonMap = new HashMap<>(2);
        //解析css列表
        Elements cssNodes = document.select("link[type=text/css]");
        List<String> cssSiteList = new ArrayList<>(cssNodes.size());
        for (Element cssNode : cssNodes) {
            cssSiteList.add(cssNode.attr("href"));
        }
        indexJsonMap.put("css", cssSiteList);

        //解析尾部html
        if (StringUtils.isEmpty(selector)) {
            selector = ".m-g-footer .site-info";
        }
        Element siteInfo = document.select(selector).first();
        indexJsonMap.put("footer", "<div class='m-g-footer'>" + siteInfo.outerHtml() + "</div>");

        kefuIndexConfigService.updateFooter(JsonUtil.toJsonString(indexJsonMap));

        return true;
    }
}
