#ifndef MATRIX
#define MATRIX

#include <array>
#include <span>
#include <string>
#include <utility> //pair
#include <vector>

/// This matrix implements a compressed sparse row matrix like described in Wikipedia:
/// https://en.wikipedia.org/wiki/Sparse_matrix#Compressed_sparse_row_(CSR,_CRS_or_Yale_format)
template <unsigned DIM> class sparseRowMatrix
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

    /// Sets a value at the given coordinates to 0.
    void reset(unsigned row, unsigned column)
    {
        auto currentRow = rowValues(row);

        for (int i = 0; i < currentRow.size(); i++)
        {
            auto [val, col] = currentRow[i];

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

  public:
    /// Initializes an empty matrix.
    sparseRowMatrix()
    {
        for (unsigned i = 0; i < DIM + 1; i++)
            rowSeparators[i] = 0;
    }

    /// Sets a value for a given coordinate.
    void set(unsigned row, unsigned column, int value)
    {
        if (value == 0)
        {
            reset(row, column);
            return;
        }

        // inserts the value at the offset i for the current row.
        auto insertAt = [&](unsigned i) {
            // insert at correct position and adjust row separator
            values.insert(values.begin() + i + rowSeparators[row], std::pair(value, column));

            for (unsigned i = row + 1; i < DIM + 1; i++)
                rowSeparators[i]++;
        };

        auto currentRow = rowValues(row);

        // iterate through the column to keep sorting
        for (int i = 0; i < currentRow.size(); i++)
        {
            auto [val, col] = currentRow[i];

            if (col == column)
            {
                currentRow[i] = std::pair(value, column);
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

        for (auto& [val, col] : cRowValues(row))
        {
            if (column == col)
                return val;
            if (column > col)
                return 0;
        }

        return 0;
    }

    size_t nonZeros()
    {
        return values.size();
    }
};

template <unsigned DIM> class coordinateListMatrix
{
  private:
    /// Sorted first by rows and then by columns.
    std::vector<std::tuple<unsigned, unsigned, int>> values;

  public:
    int get(unsigned searchRow, unsigned searchColumn) const
    {
        for (auto [row, col, val] : values)
        {
            if (row < searchRow || col < searchColumn)
                continue;
            if (col > searchColumn)
                continue;
            if (row == searchRow && col == searchColumn)
                return val;
            if (row > searchRow)
                return 0;
        }
        return 0;
    }

    void set(unsigned setRow, unsigned setColumn, int newValue)
    {
        for (int i = 0; i < values.size(); i++)
        {
            auto& [row, col, val] = values[i];
            if (row < setRow || col < setColumn)
                continue;

            if (row == setRow && col == setColumn)
            {
                // overwrite

                if (newValue == 0)
                {
                    // actually delete it
                    values.erase(values.begin() + i);
                    return;
                }

                std::get<2>(values[i]) = newValue;
            }
            else
            {
                // insert

                if (newValue == 0)
                    // actually dont
                    return;

                values.insert(values.begin() + i, std::make_tuple(setRow, setColumn, newValue));
            }
            return;
        }

        // if we reach this, values is empty
        values.push_back(std::make_tuple(setRow, setColumn, newValue));
    }

    size_t nonZeros()
    {
        return values.size();
    }
};

template <unsigned DIM> class arrayMatrix
{
  private:
    int data[DIM][DIM];

  public:
    int get(unsigned row, unsigned col) const
    {
        return data[row][col];
    }
    void set(unsigned row, unsigned col, int val)
    {
        data[row][col] = val;
    }
    size_t nonZeros()
    {
        size_t count = 0;
        for (int i = 0; i < DIM; i++)
            for (int j = 0; j < DIM; j++)
                if (data[i][j] != 0)
                    count++;

        return count;
    }
};

template <unsigned DIM> using matrix = coordinateListMatrix<DIM>;
const std::string matrixName = "coordinateList";

#endif