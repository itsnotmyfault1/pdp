function [output]=sovit(input)
%this is the solver

[a,b]=size(input);
for i=2:a-1
  for j=2:b-1
    input(i,j)=.2*(input(i,j) + input(i,j-1) + input(i-1,j) + input(i,j+1) + input(i+1,j));
  end
end

output=input;
