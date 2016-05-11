function result = g_mutation(chrom,p_mutation,lenchrom,sizetop,gen,maxgen,bound)
% chrom           input : the chromosome (i.e. the value of x)
% p_mutation      input : the probability of mutation
% lenchrom        input : the length of each variable
% sizetop         input : the size of the population 
% gen             input : current generation
% maxgen          input : maximum generation
% bound           input : the boundaries of x
% result         output : the chromosomes after cross

for i = 1:sizetop
    %determine whether to mutate or not
    if p_mutation > rand %commence to mutate
        %select 1 chromosome randomly
        p = rand;
        while p == 0
            p = rand;
        end
        idx = ceil(p*sizetop);
        while 1  %instead of while , avoid endless loop
            p = rand; %locate the position
            while p == 0
                p = rand;
            end
            position = ceil(p*sum(lenchrom));
            temp = chrom(idx,position);
            r = rand(1,2);% determine the coefficient of 'b'
            f = r(2) * (1-gen/maxgen)^2;
            if r >= 0.5
                temp = temp + (temp - bound(position,2)) * f;
            else
                temp = temp + (bound(position,1) - temp) * f;
            end
            chrom(idx,position) = temp;
            if g_judge(chrom(idx(1),:),bound) == 1
                break;
            end
        end
    end
end
result = chrom;