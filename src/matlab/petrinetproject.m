
% Start Safe n Sound by Clean Up Everything
clear;
close all;

% Get path from current file and generate absolute path
file_path = fileparts(mfilename('fullpath'));
img_path = strcat(file_path,"/../tex/img/");
dot_path = strcat(file_path,"/../dot/")

x0 = [0 1 0];

t1 = [1 0 0];
t2 = [0 1 0];
t3 = [0 0 1];

A = [1 1 -1;-1 -1 1;0 -1 1];

filename = strcat(dot_path,'petrinet_tree.dot');
Ba = dotpetree(x0,A,7,filename);
