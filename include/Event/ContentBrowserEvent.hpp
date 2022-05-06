#pragma once

#include "EditorEvents.hpp"

namespace engine{
	class ContentBrowserEvent : public EditorEvent{
		public:
			const std::filesystem::path& getPath() const {return path;}
			EDITOR_EVENT_CLASS_CATEGORY(EDITOR_EVENT_CATEGORY_CONTENT_BROWSER);

		protected:
			ContentBrowserEvent(const std::filesystem::path &path) : path{path}{}

			std::filesystem::path path;
	};

	class ContentBrowserNewSelectionEvent : public ContentBrowserEvent{
		public:
			ContentBrowserNewSelectionEvent(const std::filesystem::path &path) : ContentBrowserEvent(path) {}

			EDITOR_EVENT_CLASS_TYPE(EDITOR_EVENT_CONTENT_BROWSER_NEW_SELECTION);
	};

	class ContentBrowserFileOpenedEvent : public ContentBrowserEvent{
		public:
			ContentBrowserFileOpenedEvent(const std::filesystem::path &path) : ContentBrowserEvent(path) {}

			EDITOR_EVENT_CLASS_TYPE(EDITOR_EVENT_CONTENT_BROWSER_FILE_OPENED);
	};

	// when a directory is opened, and so the current targeted path is changed
	class ContentBrowserDirectoryOpenedEvent : public ContentBrowserEvent{
		public:
			ContentBrowserDirectoryOpenedEvent(const std::filesystem::path &path) : ContentBrowserEvent(path) {}

			EDITOR_EVENT_CLASS_TYPE(EDITOR_EVENT_CONTENT_BROWSER_DIRECTORY_OPENED);
	};

	class ContentBrowserCreatedEvent : public ContentBrowserEvent{
		public:
			ContentBrowserCreatedEvent(const std::filesystem::path &path, bool isDirectory, std::string filename="") : ContentBrowserEvent(path), directory{isDirectory}, filename{filename} {}
			inline bool isDirectory() const {return directory;}
			inline const std::string &getFilename() const {return filename;}

			EDITOR_EVENT_CLASS_TYPE(EDITOR_EVENT_CONTENT_BROWSER_CREATED);
		private:
			bool directory=false;
			std::string filename = "";
	};

	// called just before the file / folder get deleted
	class ContentBrowserRemovedEvent : public ContentBrowserEvent{
		public:
			ContentBrowserRemovedEvent(const std::filesystem::path &path) : ContentBrowserEvent(path) {}

			EDITOR_EVENT_CLASS_TYPE(EDITOR_EVENT_CONTENT_BROWSER_REMOVED);
	};

	class ContentBrowserRenamedEvent : public ContentBrowserEvent{
		public:
			ContentBrowserRenamedEvent(const std::filesystem::path &path, const std::string &name) : ContentBrowserEvent(path), name{name} {}

			inline const std::string& getFilename() const {return name;}

			EDITOR_EVENT_CLASS_TYPE(EDITOR_EVENT_CONTENT_BROWSER_RENAMED);
		private:
			std::string name;
	};

	class ContentBrowserFileOptionPopupOpenedEvent : public ContentBrowserEvent{
		public:
			ContentBrowserFileOptionPopupOpenedEvent(const std::filesystem::path &path) : ContentBrowserEvent(path){}
			EDITOR_EVENT_CLASS_TYPE(EDITOR_EVENT_CONTENT_BROWSER_FILE_OPTION_POPUP_OPENED);
	};

	class ContentBrowserDirectoryOptionPopupOpenedEvent : public ContentBrowserEvent{
		public:
			ContentBrowserDirectoryOptionPopupOpenedEvent(const std::filesystem::path &path) : ContentBrowserEvent(path){}
			EDITOR_EVENT_CLASS_TYPE(EDITOR_EVENT_CONTENT_BROWSER_DIRECTORY_OPTION_POPUP_OPEND);
	};

	class ContentBrowserVoidOptionPopupOpenedEvent : public ContentBrowserEvent{
		public:
			ContentBrowserVoidOptionPopupOpenedEvent(const std::filesystem::path &path) : ContentBrowserEvent(path){}
			EDITOR_EVENT_CLASS_TYPE(EDITOR_EVENT_CONTENT_BROWSER_VOID_OPTION_POPUP_OPENED);
	};
}