% 处理目标频率响应，使其满足线性相位条件
function H = linearPhase(H)
N = length(H);
    for i = 0 : N - 1
        if i < N / 2
            phase = -pi / N * (N - 1) * i;
            H(i+1) = abs(H(i+1)) * exp(1i * phase);
        elseif i > N / 2
            phase = pi / N * (N - 1) * (N - i);
            H(i+1) = abs(H(N - i + 1)) * exp(1i * phase);
        else
            H(i) = 0;
        end
    end
end