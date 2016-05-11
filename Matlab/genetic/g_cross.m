function result = g_cross(chrom,p_cross,lenchrom,sizetop,bound)
% chrom           input : the chromosome (i.e. the value of x)
% p_cross         input : the probability of cross
% lenchrom        input : the length of each variable
% sizetop         input : the size of the population 
% bound           input : the boundaries of x
% result         output : the chromosomes after cross

for i = 1:sizetop
    %determine whether to cross or not
    if p_cross > rand %commence to cross
        %select 2 chromosomes randomly
        p = rand(1,2);
        while prod(p) == 0
            p = rand(1,2);
        end
        idx = ceil(p*sizetop);
        while 1  %instead of while , avoid endless loop
            p = rand; %locate the position
            while p == 0
                p = rand;
            end
            position = ceil(p*sum(lenchrom));
            b = rand;% determine the coefficient of 'b'
            temp1 = chrom(idx(1),position);
            temp2 = chrom(idx(2),position);
            chrom(idx(1),position) = b * temp1 + (1 - b) * temp2;
            chrom(idx(2),position) = b * temp2 + (1 - b) * temp1;
            if g_judge(chrom(idx(1),:),bound) * g_judge(chrom(idx(2),:),bound) == 1
                break;
            end
        end
    end
end
result = chrom;