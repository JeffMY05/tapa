#include "stream.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "clang/AST/AST.h"

using std::make_shared;
using std::shared_ptr;
using std::string;
using std::unordered_map;
using std::vector;

using clang::CharSourceRange;
using clang::ClassTemplateSpecializationDecl;
using clang::CXXMemberCallExpr;
using clang::DeclRefExpr;
using clang::DiagnosticsEngine;
using clang::Expr;
using clang::MemberExpr;
using clang::QualType;
using clang::Stmt;
using clang::Type;

using llvm::dyn_cast;

// Given a CXXMemberCallExpr, returns its stream operation type as a
// StreamOpEnum (could be kNotStreamOperation).
StreamOpEnum GetStreamOp(const CXXMemberCallExpr* call_expr) {
  // Check that the caller has type tapa::stream.
  if (!IsStreamInterface(call_expr->getRecordDecl())) {
    return StreamOpEnum::kNotStreamOperation;
  }
  // Check caller name and number of arguments.
  const string callee = call_expr->getMethodDecl()->getNameAsString();
  const auto num_args = call_expr->getNumArgs();
  if (callee == "empty" && num_args == 0) {
    return StreamOpEnum::kTestEmpty;
  } else if (callee == "try_eos" && num_args == 1) {
    return StreamOpEnum::kTryEos;
  } else if (callee == "eos") {
    if (num_args == 0) {
      return StreamOpEnum::kBlockingEos;
    } else if (num_args == 1) {
      return StreamOpEnum::kNonBlockingEos;
    }
  } else if (callee == "try_peek" && num_args == 1) {
    return StreamOpEnum::kTryPeek;
  } else if (callee == "peek") {
    if (num_args == 1) {
      return StreamOpEnum::kNonBlockingPeek;
    } else if (num_args == 2) {
      return StreamOpEnum::kNonBlockingPeekWithEos;
    }
  } else if (callee == "try_read" && num_args == 1) {
    return StreamOpEnum::kTryRead;
  } else if (callee == "read") {
    if (num_args == 0) {
      return StreamOpEnum::kBlockingRead;
    } else if (num_args == 1) {
      return StreamOpEnum::kNonBlockingRead;
    } else if (num_args == 2) {
      return StreamOpEnum::kNonBlockingDefaultedRead;
    }
  } else if (callee == "try_open" && num_args == 0) {
    return StreamOpEnum::kTryOpen;
  } else if (callee == "open" && num_args == 0) {
    return StreamOpEnum::kOpen;
  } else if (callee == "full" && num_args == 0) {
    return StreamOpEnum::kTestFull;
  } else if (callee == "write" && num_args == 1) {
    return StreamOpEnum::kWrite;
  } else if (callee == "try_write" && num_args == 1) {
    return StreamOpEnum::kTryWrite;
  } else if (callee == "close" && num_args == 0) {
    return StreamOpEnum::kClose;
  } else if (callee == "try_close" && num_args == 0) {
    return StreamOpEnum::kTryClose;
  }

  return StreamOpEnum::kNotStreamOperation;
}

// Retrive information about the given streams.
void GetStreamInfo(Stmt* root, vector<StreamInfo>& streams,
                   DiagnosticsEngine& diagnostics_engine) {
  auto report_conflict = [&diagnostics_engine](
                             const CXXMemberCallExpr* call_expr,
                             const string& name, const string& role1,
                             const string& role2) {
    static const auto stream_op_conflict = diagnostics_engine.getCustomDiagID(
        clang::DiagnosticsEngine::Error,
        "tapa::stream '%0' cannot be both %1 and %2");
    auto callee = dyn_cast<MemberExpr>(call_expr->getCallee());
    assert(callee != nullptr);
    auto diagnostics_builder =
        diagnostics_engine.Report(callee->getMemberLoc(), stream_op_conflict);
    diagnostics_builder.AddSourceRange(CharSourceRange::getCharRange(
        callee->getMemberLoc(),
        callee->getMemberLoc().getLocWithOffset(
            callee->getMemberNameInfo().getAsString().size())));
    diagnostics_builder.AddString(name);
    diagnostics_builder.AddString(role1);
    diagnostics_builder.AddString(role2);
  };
  auto report_used_as = [&diagnostics_engine](
                            const CXXMemberCallExpr* call_expr,
                            const string& name, const string& role) {
    static const auto used_as = diagnostics_engine.getCustomDiagID(
        clang::DiagnosticsEngine::Note, "'%0' used as %1 here");

    auto diagnostics_builder = diagnostics_engine.Report(
        dyn_cast<MemberExpr>(call_expr->getCallee())->getMemberLoc(), used_as);
    auto callee = dyn_cast<MemberExpr>(call_expr->getCallee());
    assert(callee != nullptr);
    diagnostics_builder.AddSourceRange(CharSourceRange::getCharRange(
        callee->getMemberLoc(),
        callee->getMemberLoc().getLocWithOffset(
            callee->getMemberNameInfo().getAsString().size())));
    diagnostics_builder.AddString(name);
    diagnostics_builder.AddString(role);
  };

  for (auto stmt : root->children()) {
    if (stmt == nullptr) {
      continue;
    }
    GetStreamInfo(stmt, streams, diagnostics_engine);

    if (const auto call_expr = dyn_cast<CXXMemberCallExpr>(stmt)) {
      const string callee = call_expr->getMethodDecl()->getNameAsString();
      const string caller =
          GetNameOfFirst<DeclRefExpr>(call_expr->getImplicitObjectArgument());
      for (auto& stream : streams) {
        if (stream.name == caller) {
          const StreamOpEnum op = GetStreamOp(call_expr);

          if (static_cast<bool>(op & StreamOpEnum::kIsConsumer)) {
            if (stream.is_producer) {
              report_conflict(call_expr, stream.name, "consumer", "producer");
              for (auto expr : stream.call_exprs) {
                if (static_cast<bool>(GetStreamOp(expr) &
                                      StreamOpEnum::kIsProducer)) {
                  report_used_as(expr, stream.name, "a producer");
                }
              }
            } else if (!stream.is_consumer) {
              stream.is_consumer = true;
            }
          }
          if (static_cast<bool>(op & StreamOpEnum::kIsProducer)) {
            if (stream.is_consumer) {
              report_conflict(call_expr, stream.name, "producer", "consumer");
              for (auto expr : stream.call_exprs) {
                if (static_cast<bool>(GetStreamOp(expr) &
                                      StreamOpEnum::kIsConsumer)) {
                  report_used_as(expr, stream.name, "a consumer");
                }
              }
            } else if (!stream.is_producer) {
              stream.is_producer = true;
            }
          }
          if (static_cast<bool>(op & StreamOpEnum::kIsBlocking)) {
            if (false && stream.is_non_blocking) {
              report_conflict(call_expr, stream.name, "blocking",
                              "non-blocking");
              for (auto expr : stream.call_exprs) {
                if (static_cast<bool>(GetStreamOp(expr) &
                                      StreamOpEnum::kIsNonBlocking)) {
                  report_used_as(expr, stream.name, "non-blocking");
                }
              }
            } else if (!stream.is_blocking) {
              stream.is_blocking = true;
            }
          }
          if (static_cast<bool>(op & StreamOpEnum::kIsNonBlocking)) {
            if (false && stream.is_blocking) {
              report_conflict(call_expr, stream.name, "non-blocking",
                              "blocking");
              for (auto expr : stream.call_exprs) {
                if (static_cast<bool>(GetStreamOp(expr) &
                                      StreamOpEnum::kIsBlocking)) {
                  report_used_as(expr, stream.name, "blocking");
                }
              }
            } else if (!stream.is_non_blocking) {
              stream.is_non_blocking = true;
            }
          }
          if (static_cast<bool>(op & StreamOpEnum::kNeedPeeking)) {
            if (!stream.need_peeking) {
              stream.need_peeking = true;
            }
          }
          if (static_cast<bool>(op & StreamOpEnum::kNeedEos)) {
            if (!stream.need_eos) {
              stream.need_eos = true;
            }
          }

          stream.ops.push_back(op);
          stream.call_exprs.push_back(call_expr);

          break;
        }
      }
    }
  }
}

// Given a Stmt, find all tapa::istream and tapa::ostream operations via DFS and
// update stream_ops.
void GetTapaStreamOps(const Stmt* stmt,
                      vector<const CXXMemberCallExpr*>& stream_ops,
                      bool init = false) {
  if (stmt == nullptr) {
    return;
  }
  if (!init) {
    switch (stmt->getStmtClass()) {
      case Stmt::DoStmtClass:
      case Stmt::ForStmtClass:
      case Stmt::WhileStmtClass:
        return;
      default: {
      }
    }
  }
  for (auto child : stmt->children()) {
    GetTapaStreamOps(child, stream_ops);
  }
  if (const auto stream = dyn_cast<CXXMemberCallExpr>(stmt)) {
    if (IsStreamInterface(stream->getRecordDecl())) {
      stream_ops.push_back(stream);
    }
  }
}

// Given a Stmt, return all tapa::istream and tapa::ostream opreations via DFS.
vector<const CXXMemberCallExpr*> GetTapaStreamOps(const Stmt* stmt) {
  vector<const CXXMemberCallExpr*> stream_ops;
  GetTapaStreamOps(stmt, stream_ops, /* init = */ true);
  return stream_ops;
}

const ClassTemplateSpecializationDecl* GetTapaStreamDecl(const Type* type) {
  if (type != nullptr) {
    if (const auto record = type->getAsRecordDecl()) {
      if (const auto decl = dyn_cast<ClassTemplateSpecializationDecl>(record)) {
        if (IsStream(decl)) {
          return decl;
        }
      }
    }
  }
  return nullptr;
}

const ClassTemplateSpecializationDecl* GetTapaStreamDecl(
    const QualType& qual_type) {
  return GetTapaStreamDecl(
      qual_type.getUnqualifiedType().getCanonicalType().getTypePtr());
}

const ClassTemplateSpecializationDecl* GetTapaStreamsDecl(const Type* type) {
  if (type != nullptr) {
    if (const auto record = type->getAsRecordDecl()) {
      if (const auto decl = dyn_cast<ClassTemplateSpecializationDecl>(record)) {
        if (IsTapaType(decl, "(i|o)?streams")) {
          return decl;
        }
      }
    }
  }
  return nullptr;
}

const ClassTemplateSpecializationDecl* GetTapaStreamsDecl(
    const QualType& qual_type) {
  return GetTapaStreamsDecl(
      qual_type.getUnqualifiedType().getCanonicalType().getTypePtr());
}
