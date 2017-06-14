package com.feiniu.webim.core;

import java.util.concurrent.atomic.AtomicInteger;

/**
 * Created by wangchen on 2014/12/19.
 */
public class SeqGenerator {
    private static final int DELTA = 1;
    private AtomicInteger seqGenerator;

    public SeqGenerator() {
        this(0);
    }
    public SeqGenerator(int startSeq) {
        seqGenerator = new AtomicInteger(startSeq);
    }

    public int getNextSeq() {
        return seqGenerator.addAndGet(DELTA);
    }
}
