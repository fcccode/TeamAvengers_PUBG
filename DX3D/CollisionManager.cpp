#include "stdafx.h"
#include "CollisionManager.h"
#include "ICollidable.h"

CollisionManager::CollisionManager()
    : m_bIsRender(true)
{
}

bool CollisionManager::HasCollision(const BoxCollider& lhs, const BoxCollider& rhs)
{
    const D3DXMATRIXA16& A_transform = lhs.GetTransform();
    const D3DXMATRIXA16& B_transform = rhs.GetTransform();

    const D3DXVECTOR3& A_extent = lhs.GetExtent();
    const D3DXVECTOR3& B_extent = rhs.GetExtent();

    const D3DXVECTOR3 distance = rhs.GetCenter() - lhs.GetCenter();

    vector<D3DXVECTOR3> A_axises(3);
    vector<D3DXVECTOR3> B_axises(3);
    vector<vector<float>> dotAB(3, vector<float>(3));
    vector<vector<float>> absDotAB(3, vector<float>(3));
    vector<float> dotAD(3);
    float r0, r1, r01, r;

    // a0
    A_axises[0].x = A_transform.m[0][0];
    A_axises[0].y = A_transform.m[0][1];
    A_axises[0].z = A_transform.m[0][2];
    B_axises[0].x = B_transform.m[0][0];
    B_axises[0].y = B_transform.m[0][1];
    B_axises[0].z = B_transform.m[0][2];
    B_axises[1].x = B_transform.m[1][0];
    B_axises[1].y = B_transform.m[1][1];
    B_axises[1].z = B_transform.m[1][2];
    B_axises[2].x = B_transform.m[2][0];
    B_axises[2].y = B_transform.m[2][1];
    B_axises[2].z = B_transform.m[2][2];
    dotAB[0][0] = D3DXVec3Dot(&A_axises[0], &B_axises[0]);
    dotAB[0][1] = D3DXVec3Dot(&A_axises[0], &B_axises[1]);
    dotAB[0][2] = D3DXVec3Dot(&A_axises[0], &B_axises[2]);
    dotAD[0] = D3DXVec3Dot(&A_axises[0], &distance);
    absDotAB[0][0] = abs(dotAB[0][0]);
    absDotAB[0][1] = abs(dotAB[0][1]);
    absDotAB[0][2] = abs(dotAB[0][2]);
    r = abs(dotAD[0]);
    r1 = B_extent.x * absDotAB[0][0] 
       + B_extent.y * absDotAB[0][1] 
       + B_extent.z * absDotAB[0][2];
    r01 = A_extent.x + r1;
    if (r > r01)
        return false;

    // a1
    A_axises[1].x = A_transform.m[1][0];
    A_axises[1].y = A_transform.m[1][1];
    A_axises[1].z = A_transform.m[1][2];
    dotAB[1][0] = D3DXVec3Dot(&A_axises[1], &B_axises[0]);
    dotAB[1][1] = D3DXVec3Dot(&A_axises[1], &B_axises[1]);
    dotAB[1][2] = D3DXVec3Dot(&A_axises[1], &B_axises[2]);
    dotAD[1] = D3DXVec3Dot(&A_axises[1], &distance);
    absDotAB[1][0] = abs(dotAB[1][0]);
    absDotAB[1][1] = abs(dotAB[1][1]);
    absDotAB[1][2] = abs(dotAB[1][2]);
    r = abs(dotAD[1]);
    r1 = B_extent.x * absDotAB[1][0] 
       + B_extent.y * absDotAB[1][1] 
       + B_extent.z * absDotAB[1][2];
    r01 = A_extent.y + r1;
    if (r > r01)
        return false;

    // a2
    A_axises[2].x = A_transform.m[2][0];
    A_axises[2].y = A_transform.m[2][1];
    A_axises[2].z = A_transform.m[2][2];
    dotAB[2][0] = D3DXVec3Dot(&A_axises[2], &B_axises[0]);
    dotAB[2][1] = D3DXVec3Dot(&A_axises[2], &B_axises[1]);
    dotAB[2][2] = D3DXVec3Dot(&A_axises[2], &B_axises[2]);
    dotAD[2] = D3DXVec3Dot(&A_axises[2], &distance);
    absDotAB[2][0] = abs(dotAB[2][0]);
    absDotAB[2][1] = abs(dotAB[2][1]);
    absDotAB[2][2] = abs(dotAB[2][2]);
    r = abs(dotAD[2]);
    r1 = B_extent.x * absDotAB[2][0]
       + B_extent.y * absDotAB[2][1] 
       + B_extent.z * absDotAB[2][2];
    r01 = A_extent.z + r1;
    if (r > r01)
        return false;

    // b0
    r = abs(D3DXVec3Dot(&B_axises[0], &distance));
    r0 = A_extent.x * absDotAB[0][0] 
       + A_extent.y * absDotAB[1][0] 
       + A_extent.z * absDotAB[2][0];
    r01 = r0 + B_extent.x;
    if (r > r01)
        return false;

    // b1
    r = abs(D3DXVec3Dot(&B_axises[1], &distance));
    r0 = A_extent.x * absDotAB[0][1] 
       + A_extent.y * absDotAB[1][1] 
       + A_extent.z * absDotAB[2][1];
    r01 = r0 + B_extent.y;
    if (r > r01)
        return false;

    // b2
    r = abs(D3DXVec3Dot(&B_axises[2], &distance));
    r0 = A_extent.x * absDotAB[0][2] 
       + A_extent.y * absDotAB[1][2] 
       + A_extent.z * absDotAB[2][2];
    r01 = r0 + B_extent.z;
    if (r > r01)
        return false;

    // a0 x b0
    r = abs(dotAD[2] * dotAB[1][0] - dotAD[1] * dotAB[2][0]);
    r0 = A_extent.y * absDotAB[2][0] 
       + A_extent.z * absDotAB[1][0];
    r1 = B_extent.y * absDotAB[0][2] 
       + B_extent.z * absDotAB[0][1];
    r01 = r0 + r1;
    if (r > r01)
        return false;

    // a0 x b1
    r = abs(dotAD[2] * dotAB[1][1] - dotAD[1] * dotAB[2][1]);
    r0 = A_extent.y * absDotAB[2][1] 
       + A_extent.z * absDotAB[1][1];
    r1 = B_extent.x * absDotAB[0][2] 
       + B_extent.z * absDotAB[0][0];
    r01 = r0 + r1;
    if (r > r01)
        return false;

    // a0 x b2
    r = abs(dotAD[2] * dotAB[1][2] - dotAD[1] * dotAB[2][2]);
    r0 = A_extent.y * absDotAB[2][2]
       + A_extent.z * absDotAB[1][2];
    r1 = B_extent.x * absDotAB[0][1]
       + B_extent.y * absDotAB[0][0];
    r01 = r0 + r1;
    if (r > r01)
        return false;

    // a1 x b0
    r = abs(dotAD[0] * dotAB[2][0] - dotAD[2] * dotAB[0][0]);
    r0 = A_extent.x * absDotAB[2][0]
       + A_extent.z * absDotAB[0][0];
    r1 = B_extent.y * absDotAB[1][2]
       + B_extent.z * absDotAB[1][1];
    r01 = r0 + r1;
    if (r > r01)
        return false;

    // a1 x b1
    r = abs(dotAD[0] * dotAB[2][1] - dotAD[2] * dotAB[0][1]);
    r0 = A_extent.x * absDotAB[2][1]
       + A_extent.z * absDotAB[0][1];
    r1 = B_extent.x * absDotAB[1][2]
       + B_extent.z * absDotAB[1][0];
    r01 = r0 + r1;
    if (r > r01)
        return false;

    // a1 x b2
    r = abs(dotAD[0] * dotAB[2][2] - dotAD[2] * dotAB[0][2]);
    r0 = A_extent.x * absDotAB[2][2]
       + A_extent.z * absDotAB[0][2];
    r1 = B_extent.x * absDotAB[1][1]
       + B_extent.y * absDotAB[1][0];
    r01 = r0 + r1;
    if (r > r01)
        return false;

    // a2 x b0
    r = abs(dotAD[1] * dotAB[0][0] - dotAD[0] * dotAB[1][0]);
    r0 = A_extent.x * absDotAB[1][0]
       + A_extent.y * absDotAB[0][0];
    r1 = B_extent.y * absDotAB[2][2]
       + B_extent.z * absDotAB[2][1];
    r01 = r0 + r1;
    if (r > r01)
        return false;

    // a2 x b1
    r = abs(dotAD[1] * dotAB[0][1] - dotAD[0] * dotAB[1][1]);
    r0 = A_extent.x * absDotAB[1][1]
       + A_extent.y * absDotAB[0][1];
    r1 = B_extent.x * absDotAB[2][2]
       + B_extent.z * absDotAB[2][0];
    r01 = r0 + r1;
    if (r > r01)
        return false;

    // a2 x b2
    r = abs(dotAD[1] * dotAB[0][2] - dotAD[0] * dotAB[1][2]);
    r0 = A_extent.x * absDotAB[1][2]
       + A_extent.y * absDotAB[0][2];
    r1 = B_extent.x * absDotAB[2][1]
       + B_extent.y * absDotAB[2][0];
    r01 = r0 + r1;
    if (r > r01)
        return false;

    return true;
}

bool CollisionManager::HasCollision(const BoxCollider& lhs, const SphereCollider& rhs)
{
    return false;
}

bool CollisionManager::HasCollision(const SphereCollider& lhs, const BoxCollider& rhs)
{
    return false;
}

bool CollisionManager::HasCollision(const SphereCollider& lhs, const SphereCollider& rhs)
{
    return false;
}

void CollisionManager::Init()
{
}

void CollisionManager::Destroy()
{
}

void CollisionManager::Update()
{
    NotifyCollision();
}

void CollisionManager::Render()
{
    if (!m_bIsRender) return;

    for (auto& c : m_usetICollidable)
    {
        ColliderBase* collider = c->GetCollider();
        if (!collider) continue;

        collider->Render();
    }
}

void CollisionManager::AddICollidable(ICollidable& val)
{
    m_usetICollidable.emplace(&val);
}

void CollisionManager::RemoveICollidable(ICollidable& val)
{
    m_usetICollidable.erase(&val);
}

void CollisionManager::NotifyCollision()
{
    if (m_usetICollidable.size() < 2) return;

    for (auto it1 = m_usetICollidable.begin(); it1 != prev(m_usetICollidable.end()); ++it1)
    {
        ICollidable* o1 = *it1;
        if (!o1) continue;

        ColliderBase* c1 = o1->GetCollider();
        if (!c1) continue;

        for (auto it2 = next(m_usetICollidable.begin()); it2 != m_usetICollidable.end(); ++it2)
        {
            ICollidable* o2 = *it2;
            if (!o2) continue;

            ColliderBase* c2 = o2->GetCollider();
            if (!c2) continue;

            if (c1->GetType() == ColliderBase::Type::kBox &&
                c2->GetType() == ColliderBase::Type::kBox)
            {
                const BoxCollider* a = static_cast<BoxCollider*>(c1);
                const BoxCollider* b = static_cast<BoxCollider*>(c2);

                if (HasCollision(*a, *b))
                {
                    o1->OnCollision(*o2);
                    o2->OnCollision(*o1);
                }
            }
            else if (c1->GetType() == ColliderBase::Type::kBox &&
                     c2->GetType() == ColliderBase::Type::kSphere)
            {
                const BoxCollider* a = static_cast<BoxCollider*>(c1);
                const SphereCollider* b = static_cast<SphereCollider*>(c2);

                if (HasCollision(*a, *b))
                {
                    o1->OnCollision(*o2);
                    o2->OnCollision(*o1);
                }
            }
            else if (c1->GetType() == ColliderBase::Type::kSphere &&
                     c2->GetType() == ColliderBase::Type::kBox)
            {
                const SphereCollider* a = static_cast<SphereCollider*>(c1);
                const BoxCollider* b = static_cast<BoxCollider*>(c2);

                if (HasCollision(*a, *b))
                {
                    o1->OnCollision(*o2);
                    o2->OnCollision(*o1);
                }
            }
            else if (c1->GetType() == ColliderBase::Type::kSphere &&
                     c2->GetType() == ColliderBase::Type::kSphere)
            {
                const SphereCollider* a = static_cast<SphereCollider*>(c1);
                const SphereCollider* b = static_cast<SphereCollider*>(c2);

                if (HasCollision(*a, *b))
                {
                    o1->OnCollision(*o2);
                    o2->OnCollision(*o1);
                }
            }
            else
            {
                // something error
            }
        }
    }
}

void CollisionManager::SetIsRender(const bool val)
{
    m_bIsRender = val;
}
