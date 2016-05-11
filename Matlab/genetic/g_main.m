maxgen = 100; % the maximum of generation 
sizetop = 20; % the size of population
lenchrom = [1 1 1 1 1]; % the length of each variable
bound = [0 2;0 2;0 2;0 2;0 2]; % the boundary of x
p_cross = 0.6;
p_mutation = 0.01;

% initialize the population
chrom = rand(sizetop,sum(lenchrom));
fitness = zeros(sizetop,1);
averageV = zeros(maxgen,1);
bestV = zeros(maxgen,1);

%evolution
for i = 1:maxgen
    chrom = g_select(fitness,chrom,sizetop);%select
    chrom = g_cross(chrom,p_cross,lenchrom,sizetop,bound);%cross
    chrom = g_mutation(chrom,p_mutation,lenchrom,sizetop,i,maxgen,bound);%mutation
    
  %  if mod(i,10) == 0
  %      chrom = g_nonliner(chrom,sizetop,bound);
  %  end
    
    for j = 1:sizetop
        fitness(j) = y(chrom(j,:));
    end
    averageV(i) = mean(fitness);
    bestV(i) = min(fitness);
end
trace = [(1:maxgen)' averageV bestV];