/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <unitedb/dbtypes.hpp>
#include <unitedb/typebase.hpp>
#include <unitedb/undo.hpp>
#include <unitedb/dbundo.hpp>
#include <unitedb/dbundoop.hpp>
#include <vector>

namespace unitedb
{

	class UndoManager;

	class FUndo : public IGenericUndo
	{
	public:
		typedef uint64_t UndoID;
		virtual void Undo() override;
		virtual void Cancel() override;
		virtual void Combine() override;

		virtual DBRevision GetRevision() const override
		{
			return revision_;
		}

		inline UndoID GetID() const
		{
			return id_;
		}

		friend class UndoManager;
	private:
		FUndo(UndoManager* owner, DBRevision rev);
		DBRevision revision_ = 0;
		UndoManager* owner_ = nullptr;
		bool valid_ = true;
		UndoID id_ = 0;
		static UndoID scounter_;
	};

	struct UndoRecord
	{
		typedef int32_t IndexType;
		UndoRecord(FUndo* undo, IndexType beg)
		{
			id_ = undo->GetID();
			rev_ = undo->GetRevision();
			begin_ = beg;
		}
		UndoRecord() = default;
		IndexType begin_ = 0;
		FUndo::UndoID id_ = 0;
		DBRevision rev_ = InvalidRevision;
	};

	typedef MappedVector<UndoRecord> UndoRecords;

	class UndoOpStack
	{
	public:
		typedef MappedUndo<UndoOp> StackType;
		UndoOpStack(DefAlloc alloc)
			: cache_(alloc)
			, records_(alloc)
		{

		}

		StackType cache_;
		UndoRecords records_;
		DBRevision last_commit_ = -1;
		DBRevision undo_counter_ = 0;
	};

	class UndoManager
	{
	public:
		template<typename Alloc>
		UndoManager(IDatabase* owner, const Alloc& cc)
			: owner_(owner)
		{
		}

		void Init();

		IGenericUndo* StartUndo();

		void PushUndo(const UndoOpArg& arg);

		void LastUpdateFailure(ObjIDCode id);

		DBRevision TopRevision() const
		{
			return stack_->undo_counter_;
		}

		void OnUndo(FUndo* undo);

		void OnCombine(FUndo* undo);

		void OnCancel(FUndo* undo);

	private:

		inline const UndoRecords& getRecords() const
		{
			return stack_->records_;
		}
		inline UndoRecords& getRecords()
		{
			return stack_->records_;
		}
		void undoImpl(int64_t rbegin, int64_t rend);
		IDatabase* owner_ = nullptr;
		UndoOpStack* stack_ = nullptr;
	};

};