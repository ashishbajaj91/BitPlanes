function [keep, epsilon, lambda, wts] = getparameters(Iref)
    [typestr,epsilon,lambda] = SetParameters();    
    type = GetType(typestr);
    keep = getKeep(type);
    wts = getweight(Iref);
end

function [type,epsilon,lambda] = SetParameters()
type        =   'projective';
epsilon     =    1e-3;
lambda      =    1e-6;
end

function [wts] = getweight(Iref)
    wts=[1 1 1.0204 .03125 1.0313 0.0204 .00055516 .00055516];
    s=sqrt(numel(Iref))/128;
    wts=[wts(1:2) wts(3)^(1/s) wts(4)/s wts(5)^(1/s) wts(6)/s wts(7:8)/(s*s)];
end

function type = GetType(typestr)
    id=find(strcmpi(typestr,{'translation','rigid','similarity','affine',...
    'rotation','projective'})); 
    type={1:2,[1:2 6],[1:3 6],1:6,6:8,1:8}; type=type{id};
end

function keep = getKeep(type)
    keep=zeros(1,8); 
    keep(type)=1; 
    keep=keep>0;
end