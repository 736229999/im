package com.feiniu.webim.filter;

import com.feiniu.webim.component.AuthService;
import com.feiniu.webim.util.web.ServletRequestUtil;
import org.apache.log4j.Logger;

import javax.servlet.*;
import javax.servlet.annotation.WebFilter;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;

/**
 * Created by zhangmin on 2015/12/9.
 */
@WebFilter(urlPatterns = "/crp")
public class ClientRightRequestFilter implements  Filter {
    private final Logger logger = Logger.getLogger(ClientRightRequestFilter.class);

    AuthService authService = AuthService.instance;
    @Override
    public void init(FilterConfig filterConfig) throws ServletException {
    }

    @Override
    public void doFilter(ServletRequest servletRequest, ServletResponse servletResponse, FilterChain filterChain)
            throws IOException, ServletException {
        HttpServletRequest httpServletRequest = (HttpServletRequest)servletRequest;

        String serviceId = ServletRequestUtil.getString(httpServletRequest,"serviceId");
        if(authService.authCookieToken(httpServletRequest,serviceId)){
            filterChain.doFilter(servletRequest, servletResponse);
        }
        else {
            ((HttpServletResponse) servletResponse).setStatus(HttpServletResponse.SC_NOT_FOUND);
        }
    }

    @Override
    public void destroy() {
        if (logger.isDebugEnabled()) {
        }
    }


}
