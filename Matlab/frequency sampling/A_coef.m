function [A] = A_coef(N, omega, k)
    function [S] = S(omega)
        if omega == 0
            S = N;
        else if omega == pi
            S = N * (-1)^N;
        else
            S = sin(N * omega) / sin(omega);
        end
        end
    end

    omega_1 = omega / 2 - pi * k / N;
    omega_2 = omega / 2 + pi * k / N;
    S_1 = S(omega_1);
    S_2 = S(omega_2);
    
    A = 1 / N * (S_1 + S_2);
end