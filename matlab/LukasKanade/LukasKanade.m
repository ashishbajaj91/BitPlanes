function [H] = LukasKanade(I, Iref, H, Ds, Mref, K, wts, keep, epsilon, lambda, rect)
    [h,w] = size(I); 
    h = h-2;
    w = w-2;

    %Run the SD
    lambda=lambda*w*h*eye(K); 
	ds=zeros(1,8); 
	err=inf;
    for i=1:100
      %s=svd(H); 
      %if(s(3)<=1e-4*s(1)), H=eye(3); return; end
      Ip=warpA( I, H, size(I) );
      dI = zeros(size(Iref));
      dI(rect(1):rect(3),rect(2):rect(4))=Ip(rect(1):rect(3),rect(2):rect(4))-Iref(rect(1):rect(3),rect(2):rect(4)); 
      dI0=abs(dI);
      M=Mref & ~isnan(Ip); 
      M0=M; 
      M1=find(M); 
	  D=Ds(M1,:); 
      ds1=(D'*D + lambda)^(-1)*(D'*dI(M1));
      if(any(isnan(ds1))), ds1=zeros(K,1); end
      ds(keep)=ds1; 
      H1=ds2H(ds,wts); H=H*H1; H=H/H(9);
      err0=err; 
      err=dI0; 
      err(~M0)=0; 
      err=mean2(err); 
      del=err0-err;
      if( del<epsilon ), break; end
    end
end