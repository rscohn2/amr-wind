#ifndef VELOCITY_BCS_H
#define VELOCITY_BCS_H

#include "amr-wind/core/FieldBCOps.H"
#include "amr-wind/core/FieldFillPatchOps.H"
#include "amr-wind/core/FieldRepo.H"
#include "amr-wind/core/SimTime.H"
#include "amr-wind/physics/udfs/LinearProfile.H"
#include "amr-wind/physics/udfs/PowerLawProfile.H"

namespace amr_wind {
namespace vel_bc {

template <typename WallOp>
void register_inflow_vel_dirichlet(
    Field& field,
    const std::string& inflow_udf,
    const amrex::AmrCore& mesh,
    const SimTime& time)
{
    if (inflow_udf == "LinearProfile") {
        using InflowOp = BCOpCreator<udf::LinearVelProfile, WallOp>;
        field.register_fill_patch_op<FieldFillPatchOps<InflowOp>>(
            mesh, time, InflowOp(field));
    } else if (inflow_udf == "PowerLawProfile") {
        using InflowOp = BCOpCreator<udf::PowerLawProfile, WallOp>;
        field.register_fill_patch_op<FieldFillPatchOps<InflowOp>>(
            mesh, time, InflowOp(field));
    } else {
        amrex::Abort("Velocity BC: Invalid dirichlet BC type = " + inflow_udf);
    }
}

void register_velocity_dirichlet(
    Field& field,
    const amrex::AmrCore& mesh,
    const SimTime& time,
    std::pair<const std::string, const std::string> udfs)
{
    const std::string inflow_udf = udfs.first;
    const std::string wall_udf = udfs.second;

    if ((inflow_udf == "ConstDirichlet") && (wall_udf == "ConstDirichlet"))
        return;

    if (wall_udf != "ConstDirichlet") {
        amrex::Abort(
            "Velocity BC: Only constant dirichlet supported for Wall BC");
    }

    register_inflow_vel_dirichlet<ConstDirichlet>(
        field, inflow_udf, mesh, time);
}

} // namespace vel_bc
} // namespace amr_wind

#endif /* VELOCITY_BCS_H */
