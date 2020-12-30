function [H, theta] = interpolate(H_sample, N)
    H = zeros(1, N);
    theta = zeros(1, N);
    omega = (0 : N-1) / N * pi; 
    len = length(H_sample);
    for i = 0:N-1
        H(i+1) = H_sample(1) * A_coef(len, omega(i+1), 0) / 2;
        w(i+1) = -omega(i+1) * (len - 1) / 2;
        for k = 1:floor((len-1)/2)
            H(i+1) = H(i+1) + H_sample(k+1) * A_coef(len, omega(i+1), k);
        end
    end
end