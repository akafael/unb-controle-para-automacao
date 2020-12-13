function x2 = petristate(x0,A,t)
    isEnabled = all(x0(t*A<0));
    if isEnabled
        x2 = x0 + t*A;
        x2 = x2.*(x2>0);
    else
        error('Petri Net error: Transition [%s] not enabled from [%s]',...
              num2str(t),num2str(x0))
    end
end