#ifndef MATRIX
#define MATRIX

#include <array>
#include <span>
#include <utility> //pair
#include <vector>

/// This matrix implements a compressed sparse row matrix like described in Wikipedia:
/// https://en.wikipedia.org/wiki/Sparse_matrix#Compressed_sparse_row_(CSR,_CRS_or_Yale_format)
template <unsigned DIM> class matrix
{
  private:
    /// Values are sorted for each row by their column.
    std::vector<std::pair<int, unsigned>> values;
    std::array<int, DIM + 1> rowSeparators;

    /// Gets a span for the given row which can be used for reading and writing.
    std::span<std::pair<int, unsigned>> rowValues(unsigned row)
    {
        return std::span(values.begin() + rowSeparators[row], values.begin() + rowSeparators[row + 1]);
    }

    /// Gets a span for the given row which can only be used for reading.
    std::span<const std::pair<int, unsigned>> cRowValues(unsigned row) const
    {
        return std::span(values.cbegin() + rowSeparators[row], values.cbegin() + rowSeparators[row + 1]);
    }

  public:
    /// Initializes an empty matrix.
    matrix()
    {
        for (unsigned i = 0; i < DIM + 1; i++)
            rowSeparators[i] = 0;
    }

    /// Sets a value at the given coordinates to 0.
    void reset(unsigned row, unsigned column)
    {
        auto currentColumn = rowValues(row);

        for (int i = 0; i < currentColumn.size(); i++)
        {
            auto [val, col] = currentColumn[i];

            if (column == col)
            {
                // delete value and correct row separators.
                values.erase(values.begin() + i + rowSeparators[row]);
                for (int i = row + 1; i < DIM + 1; i++)
                    rowSeparators[row]--;

                return;
            }

            // the value will not be encountered, due to sorting.
            if (column > col)
                return;
        }
    }

    /// Sets a value for a given coordinate.
    void set(unsigned row, unsigned column, int value)
    {
        if (value == 0)
        {
            reset(row, column);
            return;
        }

        auto currentColumn = rowValues(row);

        // inserts the value at the offset i for the current row.
        auto insertAt = [&](unsigned i) {
            // insert at correct position and adjust row separator
            values.insert(values.begin() + i + rowSeparators[row], std::pair(value, column));

            for (unsigned i = row + 1; i < DIM + 1; i++)
                rowSeparators[i]++;
        };

        // iterate through the column to keep sorting
        for (int i = 0; i < currentColumn.size(); i++)
        {
            auto [val, col] = currentColumn[i];

            if (col == column)
            {
                currentColumn[i] = std::pair(value, column);
                return;
            }
            if (column > col)
            {
                insertAt(i);
                return;
            }
        }

        // if this is reached, the row did not contain elements
        insertAt(0);
    }

    /// Gets a value for a row and a column.
    int get(unsigned row, unsigned column) const
    {

        for (auto &[val, col] : cRowValues(row))
        {
            if (column == col)
                return val;
            if (column > col)
                return 0;
        }

        return 0;
    }

    /// Counts all values which are not zero.
    size_t nonZeros() const
    {
        return values.size();
    }
};

#endif