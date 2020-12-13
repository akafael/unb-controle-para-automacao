function B = dotpetree(x0,A,depth,filename)
%% DOTEPETREE Generate dot diagram for a given Petri Net
    B = {};
    B{1} = x0;
    seq = 1:size(A,1);

    % Create File
    fileID = fopen(filename,'w');

    % Print Header
    fprintf(fileID,'digraph G {\n\tnode [shape=rectangle,style=filled,color=white];\n');

    % Print First Node
    fprintf(fileID,'\n\tc%d_%d [label = "[%s]"]\n',1,1,num2str(x0));

    % Print Tree Nodes
    template = '\tc%d_%d [label = "[%s]"]\n\tc%d_%d -> c%d_%d [label = "%s"]\n';
    for i = 2:depth
        B{i} = {};
        if iscell(B{i-1})
            k = 0;
            for j = 1:size(B{i-1},2)
                xi = B{i-1}{j};
                for ts = seq
                    t = seq==ts;
                    isEnabled = all(xi((t*A)<0));
                    if isEnabled
                        k = k+1;
                        B{i}{end+1} = xi + t*A;
                        fprintf(fileID,template,...
                                i,k,num2str(xi + t*A),...
                                (i-1),j,i,k,...
                                strcat("t",num2str(ts))....
                                );
                    end
                end
            end
        else
            xi = B{i-1};
            k = 0;
            for ts = seq
                t = seq==ts;
                isEnabled = all(xi((t*A)<0));
                j=1;
                if isEnabled
                    k = k+1;
                    B{i}{end+1} = xi + t*A;
                    fprintf(fileID,template,...
                            i,k,num2str(xi + t*A),...
                            (i-1),j,i,k,...
                            strcat("t",num2str(ts))....
                            );
                end
            end
        end
    end

    fprintf(fileID,"\n}\n");
    fclose(fileID);
end
