package com.feiniu.webim.filter;

import com.feiniu.webim.util.general.IPUtil;
import com.feiniu.webim.util.json.JSONUtil;
import org.apache.log4j.Logger;

import javax.servlet.*;
import javax.servlet.annotation.WebFilter;
import javax.servlet.http.HttpServletRequest;
import java.io.IOException;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.atomic.AtomicInteger;

/**
 * Created by wangchen on 2014/12/31.
 */
@WebFilter(urlPatterns = "/im")
public class RequestStatisticsFilter implements Filter{
    private final ConcurrentHashMap<String, AtomicInteger> statisticsMap = new ConcurrentHashMap<>();
    private final Logger logger = Logger.getLogger(RequestStatisticsFilter.class);

    @Override
    public void init(FilterConfig filterConfig) throws ServletException {
    }

    @Override
    public void doFilter(ServletRequest servletRequest, ServletResponse servletResponse, FilterChain filterChain)
            throws IOException, ServletException {
        HttpServletRequest httpServletRequest = (HttpServletRequest)servletRequest;

        String requestIP = IPUtil.getIpAddr(httpServletRequest);
        if (! statisticsMap.containsKey(requestIP)) {
            statisticsMap.put(requestIP, new AtomicInteger(0));
        }
        statisticsMap.get(requestIP).addAndGet(1);

        filterChain.doFilter(servletRequest, servletResponse);
    }

    @Override
    public void destroy() {
        if (logger.isDebugEnabled()) {
            logger.debug("Tomcat statistics: " + JSONUtil.toJsonString(statisticsMap));
        }
    }
}
