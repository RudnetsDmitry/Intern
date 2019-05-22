/// @file	ObjectsHolderDecl.h
///
/// @brief interfaces for work with galery entities
///
/// @author	RDN © Intern :)
///
/// @date   22.03.2019   08:51
///

#pragma once
#include <vector>
#include <memory>

namespace gallery
{
	class Album;
	class Picture;

	//////////////////////////////////////////////////////////////////////////
	/// interface for process items
	//////////////////////////////////////////////////////////////////////////
	template<class TItem>
	struct IItemsManager
	{
		using item_t = TItem;

		virtual ~IItemsManager() {}

		virtual void AddItem(TItem &) = 0;
		virtual void UpdateItem(TItem const &) = 0;
		virtual void RemoveItem(int itemId) = 0;
		virtual std::vector<std::unique_ptr<item_t>> GetAllItems() = 0;
	};

	using IAlbumManager = IItemsManager<Album>;
	using IPictureManager = IItemsManager<Picture>;

	struct IGalleryManager
	{
		virtual ~IGalleryManager() {}

		virtual IAlbumManager & GetAlbumManager() = 0;
		virtual IPictureManager & GetPictureManager() = 0;
	};
}
