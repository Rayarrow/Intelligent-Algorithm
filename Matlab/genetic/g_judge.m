function flag = g_judge(vector,bound)
% vector        input : a vector to be judged
% bound         input : the boundaries of x
% flag         output : the flag
flag = 1;
for i = 1:length(vector)
    if vector(i) > bound(i,2) || vector(i) < bound(i,1)
        flag = 0;
        break;
    end
end