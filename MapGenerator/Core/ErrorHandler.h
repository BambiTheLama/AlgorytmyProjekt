#pragma once

/// <summary>
/// Klasa odpowiadaj�ca za obs�ug� log�w i wy�wietlanie b��d�w.
/// </summary>
class ErrorHandler {
public:
	/// <summary>
	/// Wy�wietla komunikat systemowy z b��dem.
	/// </summary>
	/// <param name="title">tytu� okienka b��du</param>
	/// <param name="description">opis b��du</param>
	static void ShowErrorMsgBox(const char* title, const char* description);
	/// <summary>
	/// Dopisuje na koniec pliku w nowej linii wiadomo�� z logiem opatrzona dat�.
	/// Je�eli plik nie istnieje jest tworzony.
	/// </summary>
	/// <param name="fileLocation">�cie�ka do pliku</param>
	/// <param name="message">wiadomo�� logu</param>
	static void LogErrorToFile(const char* fileLocation, const char* message);
};
