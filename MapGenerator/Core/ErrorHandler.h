#pragma once

/// <summary>
/// Klasa odpowiadaj¹ca za obs³ugê logów i wyœwietlanie b³êdów.
/// </summary>
class ErrorHandler {
public:
	/// <summary>
	/// Wyœwietla komunikat systemowy z b³êdem.
	/// </summary>
	/// <param name="title">tytu³ okienka b³êdu</param>
	/// <param name="description">opis b³êdu</param>
	static void ShowErrorMsgBox(const char* title, const char* description);
	/// <summary>
	/// Dopisuje na koniec pliku w nowej linii wiadomoœæ z logiem opatrzona dat¹.
	/// Je¿eli plik nie istnieje jest tworzony.
	/// </summary>
	/// <param name="fileLocation">œcie¿ka do pliku</param>
	/// <param name="message">wiadomoœæ logu</param>
	static void LogErrorToFile(const char* fileLocation, const char* message);
};
