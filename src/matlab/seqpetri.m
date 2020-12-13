function x2 = seqpetri(x0,A,ts)
    x2 = x0;
    for t = ts
        x2 = petristate(x2,A,t{1});
    end
end