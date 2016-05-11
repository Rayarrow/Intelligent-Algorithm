function result = g_select(fitness,chrom,sizetop)
% fitness        input : the values of function
% chrom          input : the chromosome (i.e. the value of x)
% sizetop        input : the size of the population
% result        output : the selected chromosomes

%calculate the fitness value and probability
fitness = 1./fitness; %note : the latter "fitness" is the value of function
probability = fitness/sum(fitness);
idx = (1:sizetop)';
%roulette
for i = 1:sizetop
    p = rand; %determine the distance the wheel covers
    %give the wheel a propulsion
    for j = 1:sizetop
        p = p-probability(j);
        if p < 0
            idx(i) = j;
            break;
        end
    end
end
result = chrom(idx,:);