close all;
warning('off', 'all')

v = VideoReader('bitplanes_data/v9.mov');
rect = [100, 190, 650, 900];

figure; hold on;
H = eye(3);

count = 0;
while hasFrame(v)
    v1 = readFrame(v);
    if (count ~= 0)
        I = double(rgb2gray(v1))/255;
        I = imgaussfilt(I,sig);
        I_bitPlane = generateBitPlanes(I);
        [I_bitPlane, ~] = padbitPlanesimages(I_bitPlane, Iref_bitPlane);
        H = LukasKanade(I_bitPlane, Iref_bitPlane, H, Ds, Mref, K, wts, keep, epsilon, lambda);
    else
        sig = 2;
        Iref = double(rgb2gray(v1))/255;
        Iref = imgaussfilt(Iref,sig);
        Iref_bitPlane = generateBitPlanes(Iref);
        [~, Iref_bitPlane] = padbitPlanesimages(Iref_bitPlane , Iref_bitPlane);
        [keep, epsilon, lambda, wts] = getparameters(Iref(:,:,1));
        [Ds, Mref, K] = ComputeDs(Iref_bitPlane, keep, wts);
        
    end
    count = count + 1
    %Draws
    DrawImage(v1, rect, H);
end
