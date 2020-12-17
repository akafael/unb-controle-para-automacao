function x2 = seqpetri(x0,A,ts)
    x2 = {x0};
    for t = ts
        x2{end+1} = petristate(x2{end},A,t{1});
    end
end