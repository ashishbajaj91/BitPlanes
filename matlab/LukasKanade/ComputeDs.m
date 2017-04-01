function [Ds, Mref, K] = ComputeDs(Iref, keep, wts) 
    % prepare subspace around Iref
    [~,Hs]=ds2H(-ones(1,8),wts); Hs=Hs(:,:,keep); K=size(Hs,3);
    [h,w]=size(Iref); 
    Ts=zeros(h,w,K); 
    k=0;
    if(keep(1)), k=k+1; Ts(:,1:end-1,k)=Iref(:,2:end); end
    if(keep(2)), k=k+1; Ts(1:end-1,:,k)=Iref(2:end,:); end
    for i=k+1:K 
        Ts(:,:,i) = warpA( Iref, Hs(:,:,i), size(Iref) );
    end
    Ds=Ts-Iref(:,:,ones(1,K)); 
    Mref = ~any(isnan(Ds),3);
    Ds = reshape(Ds,[],size(Ds,3));
end