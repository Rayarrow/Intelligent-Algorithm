function result = g_nonliner(chrom,sizetop,bound)
for i = 1:sizetop
    x = fmincon(@y,chrom(i,:),[],[],[],[],bound(:,1),bound(:,2));
    chrom(i,:) = x;
end
result = chrom;