function H = linearPhase(H)
N = length(H);
    for i = 0 : N - 1
        if (i < N / 2)
            phase = -pi / N * (N - 1) * i;
            H(i+1) = abs(H(i+1)) * exp(1i * phase);
        else if (i > N / 2)
            phase = pi / N * (N - 1) * (N - i);
            H(i+1) = abs(H(N - i + 1)) * exp(1i * phase);
        else
            phase = 0;
        end
        end
    end
end