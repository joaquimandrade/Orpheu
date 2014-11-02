
#include <filesManager.h>

#if defined(WIN32)
#include <Windows.h>
#else
#include <sys/types.h>
#include <dirent.h>
#endif
#include "configManager.h"

namespace FilesManager
{
	bool dirExists(const char *dir)
	{
#if defined WIN32 || defined _WIN32
		DWORD attr = GetFileAttributes(dir);

		if (attr == INVALID_FILE_ATTRIBUTES)
			return false;

		if (attr & FILE_ATTRIBUTE_DIRECTORY)
			return true;

#else
		struct stat s;

		if (stat(dir, &s) != 0)
			return false;

		if (S_ISDIR(s.st_mode))
			return true;
#endif

		return false;
	}

	CVector<ke::AString>* getFolders(const ke::AString& directory)
	{
		CVector<ke::AString>* folders = new CVector<ke::AString>();

		if (dirExists(directory.chars()))
		{
#if defined WIN32 || defined _WIN32
			char path[256];
			UTIL_Format(path, sizeof(path) - 1, "%s\\*", directory);

			WIN32_FIND_DATA fd;
			HANDLE hFile = FindFirstFile(path, &fd);

			if (hFile != INVALID_HANDLE_VALUE)
			{
				do
				{
					UTIL_Format(path, sizeof(path) - 1, "%s\\%s", directory, fd.cFileName);

					if (dirExists(path))
					{
						folders->push_back(ke::AString(fd.cFileName));
					}
				} while (FindNextFile(hFile, &fd));

				FindClose(hFile);
			}
#else
			char path[256];
			UTIL_Format(path, sizeof(path) - 1, "%s", directory);

			DIR *dp = opendir(dirname);

			if (dp != 0)
			{
				struct dirent *ep;

				while ((ep = readdir(dp))
				{
					UTIL_Format(path, sizeof(path) - 1, "%s/%s", directory, ep->d_name);

					if (dirExists(path))
					{
						folders->push_back(ke::AString(ep->d_name));
					}
				}

				closedir(dp);
			}
#endif
		}

		return folders;
	}
	CVector<ke::AString>* getFiles(const ke::AString& directory)
	{
		CVector<ke::AString>* files = new CVector<ke::AString>();

		if (dirExists(directory.chars()))
		{
#if defined WIN32 || defined _WIN32
			char path[256];
			UTIL_Format(path, sizeof(path) - 1, "%s\\*", directory);

			WIN32_FIND_DATA fd;
			HANDLE hFile = FindFirstFile(path, &fd);

			if (hFile != INVALID_HANDLE_VALUE)
			{
				do
				{
					UTIL_Format(path, sizeof(path) - 1, "%s\\%s", directory, fd.cFileName);

					if (!dirExists(path))
					{
						files->push_back(ke::AString(fd.cFileName));
					}
				} while (FindNextFile(hFile, &fd));

				FindClose(hFile);
			}
#else
			char path[256];
			UTIL_Format(path, sizeof(path) - 1, "%s", directory);

			DIR *dp = opendir(dirname);

			if (dp != 0)
			{
				struct dirent *ep;

				while ((ep = readdir(dp))
				{
					UTIL_Format(path, sizeof(path) - 1, "%s/%s", directory, ep->d_name);

					if (!dirExists(path))
					{
						files->push_back(ke::AString(ep->d_name));
					}
				}

				closedir(dp);
			}
#endif
		}

		return files;
	}
}

