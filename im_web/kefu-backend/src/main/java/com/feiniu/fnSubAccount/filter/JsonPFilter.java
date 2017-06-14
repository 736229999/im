package com.feiniu.fnSubAccount.filter;

import org.springframework.web.bind.ServletRequestUtils;

import javax.servlet.*;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.io.OutputStream;

/**
 * Created by wangchen on 2015/1/8.
 */
public class JsonPFilter implements Filter {
    @Override
    public void init(FilterConfig filterConfig) throws ServletException {
    }

    @Override
    public void doFilter(ServletRequest servletRequest, ServletResponse servletResponse, FilterChain filterChain) throws IOException, ServletException {
        HttpServletRequest request = (HttpServletRequest)servletRequest;
        HttpServletResponse response = (HttpServletResponse)servletResponse;

        String callback = ServletRequestUtils.getStringParameter(request, "callback");
        OutputStream out = servletResponse.getOutputStream();

        GenericResponseWrapper wrapper = new GenericResponseWrapper(response);

        if (callback != null && callback.length() > 0) {
            out.write((callback + "(").getBytes());
            out.flush();
        }

        filterChain.doFilter(servletRequest, wrapper);

        out.write(wrapper.getData());
        out.flush();
        if (callback != null && callback.length() > 0) {
            out.write((");").getBytes());
        }

        out.flush();
    }

    @Override
    public void destroy() {
    }
}
