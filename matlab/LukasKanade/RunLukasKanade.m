close all;
warning('off', 'all')

v = VideoReader('../bitplanes_data/v9.mov');
rect = [100, 190, 650, 900];

figure; hold on;
H = eye(3);

count = 0;
while hasFrame(v)
    v1 = readFrame(v);
    if (count ~= 0)
        I = double(rgb2gray(v1))/255;
        I = imgaussfilt(I,sig);
        [I, ~] = padimages(I, Iref);
        H = LukasKanade(I, Iref, H, Ds, Mref, K, wts, keep, epsilon, lambda, rect);
    else
        sig = 2;
        Iref = double(rgb2gray(v1))/255;
        Iref = imgaussfilt(Iref,sig); 
        
        [~, Iref] = padimages(Iref, Iref);
        [keep, epsilon, lambda, wts] = getparameters(Iref);
        [Ds, Mref, K] = ComputeDs(Iref, keep, wts);
        
    end
    count = count + 1
    %Draws
    DrawImage(v1, rect, H);
end
