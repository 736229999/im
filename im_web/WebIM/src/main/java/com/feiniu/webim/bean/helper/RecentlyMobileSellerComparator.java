package com.feiniu.webim.bean.helper;

import com.feiniu.webim.bean.domain.RecentlyMobileSeller;

import java.util.Comparator;

/**
 * Created by wangyixiong on 2015/11/30.
 */
public class RecentlyMobileSellerComparator implements Comparator<RecentlyMobileSeller> {
    @Override
    public int compare(RecentlyMobileSeller o1, RecentlyMobileSeller o2) {
        return o2.getLastMsgTime() - o1.getLastMsgTime();
    }
}
