#pragma once

#include <cstddef>  // size_t
#include <cstdint>
#include <xiosbase>

/// <summary>
/// Implementacja szumu Simplex Perlin.
/// </summary>
class SimplexNoise {
public:
    
    // szum 1D Perlin simplex
    static float noise(float x);
    // szun 2D Perlin simplex
    static float noise(float x, float y);
    // szum 3D Perlin simplex
    static float noise(float x, float y, float z);

    // Suma szumów Fraktalnych / Brownowskich(fBm)
    float fractal(size_t octaves, float x) const;
    float fractal(size_t octaves, float x, float y) const;
    float fractal(size_t octaves, float x, float y, float z) const;

    /**
    * Konstruktor s³u¿¹cy do inicjalizacji sumy szumów fraktalnych
    *
    * @param[in] frequency Czêstotliwoœæ ("szerokoœæ") pierwszej oktawy szumu (domyœlnie 1.0)
    * @param[in] amplitude Amplituda ("wysokoœæ") pierwszej oktawy szumu (domyœlnie 1.0)
    * @param[in] lacunarity Lacunarity okreœla mno¿nik czêstotliwoœci miêdzy kolejnymi oktawami (domyœlnie 2.0)
    * @param[in] persistence Persistence to utrata amplitudy miêdzy kolejnymi oktawami (zazwyczaj 1/lacunarity)
    */
    explicit SimplexNoise(float frequency = 1.0f,
                          float amplitude = 1.0f,
                          float lacunarity = 2.0f,
                          float persistence = 0.5f) :
        mFrequency(frequency),
        mAmplitude(amplitude),
        mLacunarity(lacunarity),
        mPersistence(persistence) {
    }

    /// <summary>
    /// Konstruktor przeci¹¿ony z parametrem okreœlaj¹cym czy losowaæ wartoœci tablicy perm (ziarna szumu).
    /// </summary>
    /// <param name="initRand">flaga okreœlaj¹ca czy losowaæ wartoœci tablicy perm (ziarna szumu)</param>
    explicit SimplexNoise(bool initRand = true) : SimplexNoise(1.0f, 1.0f, 2.0f, 0.5f) {
        srand((unsigned)time(NULL));
        if (initRand)
        {
            RandPermArray();
        }
    }

    /// <summary>
    /// Ustawia parametry szumu.
    /// </summary>
    /// <param name="frequency">Czêstotliwoœæ ("szerokoœæ") pierwszej oktawy szumu (domyœlnie 1.0)</param>
    /// <param name="amplitude">Amplituda ("wysokoœæ") pierwszej oktawy szumu (domyœlnie 1.0)</param>
    /// <param name="lacunarity">Lacunarity okreœla mno¿nik czêstotliwoœci miêdzy kolejnymi oktawami (domyœlnie 2.0)</param>
    /// <param name="persistence">Persistence to utrata amplitudy miêdzy kolejnymi oktawami (zazwyczaj 1/lacunarity)</param>
    void SetParams(float frequency = 1.0f,
                   float amplitude = 1.0f,
                   float lacunarity = 2.0f,
                   float persistence = 0.5f);

    /// <summary>
    /// Generuje pseudolosowe wartoœci dla tablicy perm.
    /// </summary>
    void RandPermArray();

    /// <summary>
    /// Zwraca wskaŸnik na tablicê perm.
    /// </summary>
    /// <returns></returns>
    uint8_t* GetPermTable();

private:
    // Parametry ruchu fraktalnego Browna (fBm): suma N "oktaw" szumu
    float mFrequency;   ///< Czêstotliwoœæ ("szerokoœæ") pierwszej oktawy szumu (domyœlnie 1.0)
    float mAmplitude;   ///< Amplituda ("wysokoœæ") pierwszej oktawy szumu (domyœlnie 1.0)
    float mLacunarity;  ///< Lacunarity okreœla mno¿nik czêstotliwoœci miêdzy kolejnymi oktawami (domyœlnie 2.0).
    float mPersistence; ///< Persistence to utrata amplitudy miêdzy kolejnymi oktawami (zazwyczaj 1/lacunarity)
};